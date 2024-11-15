/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include "sdkconfig.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_memory_utils.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_panel_utils.h"
#include "drivers/bus/esp_panel_bus.hpp"
#include "esp_panel_lcd_base.hpp"

namespace esp_panel::drivers {

bool LCD::configVendorCommands(const esp_panel_lcd_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: init_cmd(@%p), init_cmd_size(%d)", init_cmd, static_cast<int>(init_cmd_size));
    ESP_UTILS_CHECK_FALSE_RETURN((init_cmd == nullptr) || (init_cmd_size > 0), false, "Invalid arguments");

    vendor_config.init_cmds = init_cmd;
    vendor_config.init_cmds_size = init_cmd_size;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configColorRgbOrder(bool reverse_order)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: reverse_order(%d)", reverse_order);

    if (reverse_order) {
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
    } else {
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configResetActiveLevel(int level)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "This function should be called before `init()`");

    ESP_UTILS_LOGD("Param: level(%d)", level);
    panel_config.flags.reset_active_high = level;

    return true;
}

bool LCD::configMirrorByCommand(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

#if SOC_LCD_RGB_SUPPORTED
    ESP_UTILS_CHECK_FALSE_RETURN(checkBusIsValid(), false, "Invalid bus");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "This function should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(
        (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) && (std::static_pointer_cast<Bus_RGB>(bus)->checkUseSPI_Interface()), false,
        "This function is only for \"3-wire SPI + RGB\" interface"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    vendor_config.flags.mirror_by_cmd = en;
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "This function is only for \"3-wire SPI + RGB\" interface");
#endif // SOC_LCD_RGB_SUPPORTED

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configEnableIO_Multiplex(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

#if SOC_LCD_RGB_SUPPORTED
    ESP_UTILS_CHECK_FALSE_RETURN(checkBusIsValid(), false, "Invalid bus");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "This function should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(
        (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) && (std::static_pointer_cast<Bus_RGB>(bus)->checkUseSPI_Interface()), false,
        "This function is only for \"3-wire SPI + RGB\" interface"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    vendor_config.flags.enable_io_multiplex = en;
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "This function is only for \"3-wire SPI + RGB\" interface");
#endif // SOC_LCD_RGB_SUPPORTED

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun() || _flags.is_reset, false, "Already begun and not reset");

    /* Initialize LCD panel */
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_init(panel_handle), false, "Init panel failed");
    ESP_UTILS_LOGD("LCD panel(@%p) initialized", panel_handle);

    /* If the panel is reset, goto end directly */
    if (checkIsBegun() && _flags.is_reset) {
        goto end;
    }

    /* For non-RGB interface, create Semaphore for using API `drawBitmapWaitUntilFinish()` */
    if ((bus->getType() != ESP_PANEL_BUS_TYPE_RGB) && (_callback.draw_bitmap_finish_sem == nullptr)) {
        _callback.draw_bitmap_finish_sem = xSemaphoreCreateBinary();
        ESP_UTILS_CHECK_NULL_RETURN(_callback.draw_bitmap_finish_sem, false, "Create draw bitmap finish semaphore failed");
    }

    /* Register callback for different interface */
    _callback.data.lcd_ptr = this;
    switch (bus->getType()) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB: {
        esp_lcd_rgb_panel_event_callbacks_t rgb_event_cb = {};
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
        rgb_event_cb.on_frame_buf_complete = (esp_lcd_rgb_panel_frame_buf_complete_cb_t)onRefreshFinish;
#else
        const esp_lcd_rgb_panel_config_t *rgb_config = std::static_pointer_cast<Bus_RGB>(bus)->getPanelConfig();
        if (rgb_config->bounce_buffer_size_px == 0) {
            // When bounce buffer is disabled, use `on_vsync` callback to notify draw bitmap finish
            rgb_event_cb.on_vsync = (esp_lcd_rgb_panel_vsync_cb_t)onRefreshFinish;
        } else {
            // When bounce buffer is enabled, use `on_bounce_frame_finish` callback to notify draw bitmap finish
            rgb_event_cb.on_bounce_frame_finish = (esp_lcd_rgb_panel_bounce_buf_finish_cb_t)onRefreshFinish;
        }
#endif
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_rgb_panel_register_event_callbacks(panel_handle, &rgb_event_cb, &_callback.data), false,
            "Register RGB event callback failed"
        );
        break;
    }
#endif
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI: {
        esp_lcd_dpi_panel_event_callbacks_t dpi_event_cb = {
            .on_color_trans_done = (esp_lcd_dpi_panel_color_trans_done_cb_t)onDrawBitmapFinish,
            .on_refresh_done = (esp_lcd_dpi_panel_refresh_done_cb_t)onRefreshFinish,
        };
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_dpi_panel_register_event_callbacks(panel_handle, &dpi_event_cb, &_callback.data), false,
            "Register MIPI-DSI event callback failed"
        );
        break;
    }
#endif
    default:
        esp_lcd_panel_io_callbacks_t io_cb = {
            .on_color_trans_done = (esp_lcd_panel_io_color_trans_done_cb_t)onDrawBitmapFinish,
        };
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_panel_io_register_event_callbacks(bus->getIO_Handle(), &io_cb, &_callback.data), false,
            "Register panel IO event callback failed"
        );
        break;
    }

end:
    _flags.is_begun = true;
    _flags.is_reset = false;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::reset(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    // For RGB LCD, if the flag `enable_io_multiplex` is enabled, when the panel is not begun, ignore reset
    if ((bus->getType() == ESP_PANEL_BUS_TYPE_RGB) && !checkIsBegun() && vendor_config.flags.enable_io_multiplex) {
        ESP_UTILS_LOGW("Ignore `reset()` before `begin()` when using RGB LCD with flag `enable_io_multiplex` enabled");
        goto end;
    }

    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_reset(panel_handle), false, "Reset panel failed");
    ESP_UTILS_LOGD("LCD panel(@%p) reset", panel_handle);

end:
    _flags.is_reset = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsInit()) {
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_panel_del(panel_handle), false, "Delete LCD panel(@%p) failed", panel_handle
        );
        ESP_UTILS_LOGD("LCD panel(@%p) deleted", panel_handle);
        panel_handle = nullptr;
    }

    if (_callback.draw_bitmap_finish_sem) {
        vSemaphoreDelete(_callback.draw_bitmap_finish_sem);
        _callback.draw_bitmap_finish_sem = nullptr;
    }

    _flags = {};
    _state = {};
    _callback = {};

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::drawBitmap(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD(
        "Param: x_start(%d), y_start(%d), width(%d), height(%d), color_data(@%p)", x_start, y_start, width, height,
        color_data
    );
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_draw_bitmap(panel_handle, x_start, y_start, x_start + width, y_start + height, color_data),
        false, "Draw bitmap failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::drawBitmapWaitUntilFinish(
    uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data, int timeout_ms
)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: timeout_ms(%d)", timeout_ms);
    /* For RGB LCD, since `drawBitmap()` uses `memcpy()` instead of DMA operation, doesn't need to wait for finish */
    ESP_UTILS_CHECK_FALSE_RETURN(drawBitmap(x_start, y_start, width, height, color_data), false, "Draw bitmap failed");

    /* For other interfaces which uses DMA operation, wait for the drawing to finish */
    if (bus->getType() != ESP_PANEL_BUS_TYPE_RGB) {
        /* Wait for the semaphore to be given by the callback function */
        BaseType_t timeout_tick = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
        ESP_UTILS_CHECK_FALSE_RETURN(
            xSemaphoreTake(_callback.draw_bitmap_finish_sem, timeout_tick) == pdTRUE, false,
            "Draw bitmap wait for finish timeout"
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::mirrorX(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    if (_attributes.disable_mirror) {
        ESP_UTILS_LOGW("This function is not supported");
        return true;
    }

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_mirror(panel_handle, en, _state.mirror_y), false, "Mirror X failed");
    _state.mirror_x = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::mirrorY(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    if (_attributes.disable_mirror) {
        ESP_UTILS_LOGW("This function is not supported");
        return true;
    }

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_mirror(panel_handle, _state.mirror_x, en), false, "Mirror X failed");
    _state.mirror_y = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::swapXY(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    if (_attributes.disable_swap_xy) {
        ESP_UTILS_LOGW("This function is not supported");
        return true;
    }

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_swap_xy(panel_handle, en), false, "Swap XY failed");
    _state.swap_xy = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::setGapX(uint16_t gap)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    if (_attributes.disable_gap) {
        ESP_UTILS_LOGW("Set gap function is disabled");
        return true;
    }

    ESP_UTILS_LOGD("Param: gap(%d)", static_cast<int>(gap));
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_set_gap(panel_handle, gap, _state.gap_y), false, "Set X gap failed");
    _state.gap_x = gap;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::setGapY(uint16_t gap)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    if (_attributes.disable_gap) {
        ESP_UTILS_LOGW("This function is not supported");
        return true;
    }

    ESP_UTILS_LOGD("Param: gap(%d)", gap);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_set_gap(panel_handle, _state.gap_x, gap), false, "Set Y gap failed");
    _state.gap_y = gap;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::invertColor(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_invert_color(panel_handle, en), false, "Invert color failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::setDisplayOnOff(bool enable_on)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    bool is_disable = _attributes.disable_display_on_off;
    if (is_disable) {
        goto end;
    }

#if SOC_LCD_RGB_SUPPORTED
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) {
        is_disable = (bus->getIO_Handle() == nullptr) && (vendor_config.rgb_config->disp_gpio_num == -1);
        if (is_disable) {
            goto end;
        }
    }
#endif

end:
    if (is_disable) {
        ESP_UTILS_LOGW("This function is not supported");
        return true;
    }

    ESP_UTILS_LOGD("Param: enable_on(%d)", enable_on);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_disp_on_off(panel_handle, enable_on), false, "Set display on/off failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::attachDrawBitmapFinishCallback(DrawBitmapFinishCallback callback, void *user_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    ESP_UTILS_CHECK_FALSE_RETURN(
        bus->getType() != ESP_PANEL_BUS_TYPE_RGB, false, "RGB interface doesn't support this function"
    );

    ESP_UTILS_LOGD("Param: callback(@%p), user_data(@%p)", callback, user_data);
    if ((_callback.data.user_data != nullptr) && (_callback.data.user_data != user_data)) {
        ESP_UTILS_LOGW("The previous user_data(@%p) is existed, will overwrite it", _callback.data.user_data);
    }
    _callback.data.user_data = user_data;
    _callback.on_draw_bitmap_finish = callback;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::attachRefreshFinishCallback(RefreshFinishCallback callback, void *user_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    ESP_UTILS_CHECK_FALSE_RETURN(
        (bus->getType() == ESP_PANEL_BUS_TYPE_RGB) || (bus->getType() == ESP_PANEL_BUS_TYPE_MIPI_DSI), false,
        "Only RGB and MIPI-DSI interfaces support this function"
    );

    /* Check the callback function and user data placement */
    // For RGB LCD, if the "XIP on PSRAM" function is not enabled, the callback function and user data should be
    // placed in SRAM
#if (SOC_MIPI_DSI_SUPPORTED && defined(CONFIG_LCD_DSI_ISR_IRAM_SAFE)) || \
    (SOC_LCD_RGB_SUPPORTED && defined(CONFIG_LCD_RGB_ISR_IRAM_SAFE) && \
    !(defined(CONFIG_SPIRAM_RODATA) && defined(CONFIG_SPIRAM_FETCH_INSTRUCTIONS)))
    if (bus->getType() == ESP_PANEL_BUS_TYPE_RGB || bus->getType() == ESP_PANEL_BUS_TYPE_MIPI_DSI) {
        ESP_UTILS_CHECK_FALSE_RETURN(
            esp_ptr_in_iram(reinterpret_cast<const void *>(callback)), false,
            "Callback function should be placed in IRAM, add `IRAM_ATTR` before the function"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(esp_ptr_internal(user_data), false, "User data should be placed in SRAM");
    }
#endif

    ESP_UTILS_LOGD("Param: callback(@%p), user_data(@%p)", callback, user_data);
    if ((_callback.data.user_data != nullptr) && (_callback.data.user_data != user_data)) {
        ESP_UTILS_LOGW("The previous user_data(@%p) is existed, will overwrite it", _callback.data.user_data);
    }
    _callback.data.user_data = user_data;
    _callback.on_refresh_finish = callback;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::colorBarTest(uint16_t width, uint16_t height)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    int bits_per_piexl = getColorBits();
    ESP_UTILS_LOGD("LCD bits per pixel: %d", bits_per_piexl);
    ESP_UTILS_CHECK_FALSE_RETURN(bits_per_piexl > 0, false, "Invalid color bits");

    ESP_UTILS_LOGD("Param: width(%d), height(%d)", width, height);

    int bytes_per_piexl = bits_per_piexl / 8;
    int row_per_bar = height / bits_per_piexl;
    int line_count = 0;
    int res_line_count = 0;

    /* Malloc memory for a single color bar */
    uint8_t *single_bar_buf = static_cast<uint8_t *>(malloc(row_per_bar * width * bytes_per_piexl));
    ESP_UTILS_CHECK_NULL_RETURN(single_bar_buf, false, "Alloc color buffer failed");

    /* Draw color bar from top left to bottom right, the order is B - G - R */
    for (int j = 0; j < bits_per_piexl; j++) {
        for (int i = 0; i < row_per_bar * width; i++) {
            for (int k = 0; k < bytes_per_piexl; k++) {
                if ((bus->getType() == ESP_PANEL_BUS_TYPE_SPI) || (bus->getType() == ESP_PANEL_BUS_TYPE_QSPI)) {
                    // For SPI interface, the data bytes should be swapped since the data is sent by LSB first
                    single_bar_buf[i * bytes_per_piexl + k] = SPI_SWAP_DATA_TX(BIT(j), bits_per_piexl) >> (k * 8);
                } else {
                    single_bar_buf[i * bytes_per_piexl + k] = BIT(j) >> (k * 8);
                }
            }
        }
        line_count += row_per_bar;
        ESP_UTILS_CHECK_FALSE_GOTO(
            drawBitmapWaitUntilFinish(0, j * row_per_bar, width, row_per_bar, single_bar_buf), end,
            "Draw bitmap wait until finish failed"
        );
    }

    /* Fill the rest of the screen with white color */
    res_line_count = height - line_count;
    if (res_line_count > 0) {
        ESP_UTILS_LOGD("Fill the rest lines(%d) with white color", res_line_count);
        memset(single_bar_buf, 0xff, res_line_count * width * bytes_per_piexl);
        ESP_UTILS_CHECK_FALSE_GOTO(
            drawBitmapWaitUntilFinish(0, line_count, width, res_line_count, single_bar_buf), end,
            "Draw bitmap wait until finish failed"
        );
    }

end:
    free(single_bar_buf);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

#if SOC_MIPI_DSI_SUPPORTED
bool LCD::showDSI_Pattern(DSI_PatternType type)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");
    ESP_UTILS_CHECK_FALSE_RETURN(
        bus->getType() == ESP_PANEL_BUS_TYPE_MIPI_DSI, false, "Only MIPI-DSI interface supports this function"
    );

    ESP_UTILS_LOGD("Param: type(%d)", static_cast<int>(type));
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_dpi_panel_set_pattern(panel_handle, static_cast<mipi_dsi_pattern_type_t>(type)), false,
        "Set MIPI DPI pattern failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}
#endif /* SOC_MIPI_DSI_SUPPORTED */

int LCD::getColorBits(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_NULL_RETURN(bus, -1, "Invalid bus");

    int bits_per_pixel = -1;
    switch (bus->getType()) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB: {
        const esp_lcd_rgb_panel_config_t *rgb_config = std::static_pointer_cast<Bus_RGB>(bus)->getPanelConfig();
        ESP_UTILS_CHECK_NULL_RETURN(rgb_config, -1, "Invalid RGB config");
        bits_per_pixel = rgb_config->bits_per_pixel;
        break;
    }
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI: {
        const esp_lcd_dpi_panel_config_t *dpi_config = std::static_pointer_cast<Bus_DSI>(bus)->getPanelConfig();
        ESP_UTILS_CHECK_NULL_RETURN(dpi_config, -1, "Invalid MIPI DPI config");
        switch (dpi_config->pixel_format) {
        case LCD_COLOR_PIXEL_FORMAT_RGB565:
            bits_per_pixel = 16;
            break;
        case LCD_COLOR_PIXEL_FORMAT_RGB666:
            bits_per_pixel = 18;
            break;
        case LCD_COLOR_PIXEL_FORMAT_RGB888:
            bits_per_pixel = 24;
            break;
        default:
            bits_per_pixel = panel_config.bits_per_pixel;
            break;
        }
        break;
    }
#endif /* SOC_MIPI_DSI_SUPPORTED */
    default:
        break;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return bits_per_pixel;
}

void *LCD::getFrameBufferByIndex(uint8_t index)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), nullptr, "Not begun");

    ESP_UTILS_LOGD("Param: index(%d)", index);
    ESP_UTILS_CHECK_FALSE_RETURN(
        index < FRAME_BUFFER_MAX_NUM, nullptr, "Index out of range(0-%d)", FRAME_BUFFER_MAX_NUM - 1
    );

    auto bus_type = bus->getType();
    void *buffer[FRAME_BUFFER_MAX_NUM] = {};
    switch (bus_type) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB:
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_rgb_panel_get_frame_buffer(panel_handle, index + 1, &buffer[0], &buffer[1], &buffer[2]), nullptr,
            "Get RGB buffer failed"
        );
        break;
#endif
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_dpi_panel_get_frame_buffer(panel_handle, index + 1, &buffer[0], &buffer[1], &buffer[2]), nullptr,
            "Get MIPI DPI buffer failed"
        );
        break;
#endif
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(
            false, nullptr, "The Bus(%d[%s]) doesn't support this function", bus_type,
            BusFactory::getTypeString(bus_type).c_str()
        );
        break;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return buffer[index];
}

bool LCD::loadVendorConfigFromBus(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkBusIsValid(), false, "Invalid bus");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "This function should be called before `init()`");

    auto bus_type = bus->getType();
    switch (bus_type) {
    case ESP_PANEL_BUS_TYPE_SPI:
        vendor_config.flags.use_spi_interface = 1;
        break;
    case ESP_PANEL_BUS_TYPE_QSPI:
        vendor_config.flags.use_qspi_interface = 1;
        break;
#if SOC_LCD_RGB_SUPPORTED
    /* Retrieve RGB configuration from the bus and register it into the vendor configuration */
    case ESP_PANEL_BUS_TYPE_RGB:
        vendor_config.flags.use_rgb_interface = 1;
        vendor_config.rgb_config = std::static_pointer_cast<Bus_RGB>(bus)->getPanelConfig();
        break;
#endif
#if SOC_MIPI_DSI_SUPPORTED
    /* Retrieve MIPI DPI configuration from the bus and register it into the vendor configuration */
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        vendor_config.flags.use_mipi_interface = 1;
        vendor_config.mipi_config = {
            .lane_num = std::static_pointer_cast<Bus_DSI>(bus)->getHostConfig()->num_data_lanes,
            .dsi_bus = std::static_pointer_cast<Bus_DSI>(bus)->getDSI_Handle(),
            .dpi_config = std::static_pointer_cast<Bus_DSI>(bus)->getPanelConfig(),
        };
        break;
#endif
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(
            false, false, "The Bus(%d[%s]) doesn't support this function", bus_type,
            BusFactory::getTypeString(bus_type).c_str()
        );
        break;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

IRAM_ATTR bool LCD::onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx)
{
    CallbackData *callback_data = (CallbackData *)user_ctx;
    if (callback_data == nullptr) {
        return false;
    }

    LCD *lcd_ptr = (LCD *)callback_data->lcd_ptr;
    if (lcd_ptr == nullptr) {
        return false;
    }

    BaseType_t need_yield = pdFALSE;
    if (lcd_ptr->_callback.on_draw_bitmap_finish != nullptr) {
        need_yield = lcd_ptr->_callback.on_draw_bitmap_finish(lcd_ptr->_callback.data.user_data) ? pdTRUE : need_yield;
    }
    if (lcd_ptr->_callback.draw_bitmap_finish_sem != nullptr) {
        xSemaphoreGiveFromISR(lcd_ptr->_callback.draw_bitmap_finish_sem, &need_yield);
    }

    return (need_yield == pdTRUE);
}

IRAM_ATTR bool LCD::onRefreshFinish(void *panel_io, void *edata, void *user_ctx)
{
    CallbackData *callback_data = (CallbackData *)user_ctx;
    if (callback_data == nullptr) {
        return false;
    }

    LCD *lcd_ptr = (LCD *)callback_data->lcd_ptr;
    if (lcd_ptr == nullptr) {
        return false;
    }

    BaseType_t need_yield = pdFALSE;
    if (lcd_ptr->_callback.on_refresh_finish != nullptr) {
        need_yield = lcd_ptr->_callback.on_refresh_finish(lcd_ptr->_callback.data.user_data) ? pdTRUE : need_yield;
    }

    return (need_yield == pdTRUE);
}

} // namespace esp_panel::drivers
