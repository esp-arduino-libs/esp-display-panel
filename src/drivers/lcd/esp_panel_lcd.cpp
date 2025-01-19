/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include <numeric>
#include "sdkconfig.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_io.h"
#include "esp_memory_utils.h"
#include "driver/spi_master.h"
#include "esp_panel_utils.h"
#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

void LCD::BasicBusSpecification::print(std::string bus_name) const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (bus_name.empty()) {
        ESP_UTILS_LOGI("\n\t{Bus specification}");
    } else {
        ESP_UTILS_LOGI("\n\t{%s Bus specification}", bus_name.c_str());
    }
    ESP_UTILS_LOGI(
        "\n\t\t-> [x_coord_align]: %d"
        "\n\t\t-> [y_coord_align]: %d"
        "\n\t\t-> [color_bits]: %s"
        "\n\t\t-> {functions}"
        "\n\t\t\t-> [invert_color]: %d"
        "\n\t\t\t-> [mirror_x]: %d"
        "\n\t\t\t-> [mirror_y]: %d"
        "\n\t\t\t-> [swap_xy]: %d"
        "\n\t\t\t-> [gap]: %d"
        "\n\t\t\t-> [display_on_off]: %d"
        , x_coord_align
        , y_coord_align
        , getColorBitsString().c_str()
        , isFunctionValid(Function::FUNC_INVERT_COLOR)
        , isFunctionValid(Function::FUNC_MIRROR_X)
        , isFunctionValid(Function::FUNC_MIRROR_Y)
        , isFunctionValid(Function::FUNC_SWAP_XY)
        , isFunctionValid(Function::FUNC_GAP)
        , isFunctionValid(Function::FUNC_DISPLAY_ON_OFF)
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void LCD::BasicAttributes::print() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_LOGI("\n\t{Basic attributes}");
    ESP_UTILS_LOGI("\n\t\t-> [name]: %s", name);
    basic_bus_spec.print();

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

std::string LCD::BasicBusSpecification::getColorBitsString() const
{
// *INDENT-OFF*
    return std::accumulate(
        color_bits.begin(), color_bits.end(), std::string(), [](const std::string & a, int b) {
            if (b == 0) {
                return a;
            }
            return a.empty() ? std::to_string(b) : a + ", " + std::to_string(b);
        }
    );
// *INDENT-ON*
}

const LCD::DeviceFullConfig *LCD::Config::getDeviceFullConfig() const
{
    ESP_UTILS_CHECK_FALSE_RETURN(std::holds_alternative<DeviceFullConfig>(device), nullptr, "Partial config");

    return &std::get<DeviceFullConfig>(device);
}

const LCD::VendorFullConfig *LCD::Config::getVendorFullConfig() const
{
    ESP_UTILS_CHECK_FALSE_RETURN(std::holds_alternative<VendorFullConfig>(vendor), nullptr, "Partial config");

    return &std::get<VendorFullConfig>(vendor);
}

void LCD::Config::convertPartialToFull()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<DevicePartialConfig>(device)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printDeviceConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<DevicePartialConfig>(device);
        device = DeviceFullConfig{
            .reset_gpio_num = config.reset_gpio_num,
            .rgb_ele_order = static_cast<lcd_rgb_element_order_t>(config.rgb_ele_order),
            .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
            .bits_per_pixel = static_cast<uint32_t>(config.bits_per_pixel),
            .flags = {
                .reset_active_high = config.flags_reset_active_high,
            },
            .vendor_config = nullptr,
        };
    }

    if (std::holds_alternative<VendorPartialConfig>(vendor)) {
#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
        printVendorConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG
        auto &config = std::get<VendorPartialConfig>(vendor);
        vendor = VendorFullConfig{
            .init_cmds = config.init_cmds,
            .init_cmds_size = static_cast<unsigned int>(config.init_cmds_size),
            .flags = {
                .mirror_by_cmd = config.flags_mirror_by_cmd,
                .enable_io_multiplex = config.flags_enable_io_multiplex,
            },
        };
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void LCD::Config::printDeviceConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<DeviceFullConfig>(device)) {
        auto &config = std::get<DeviceFullConfig>(device);
        ESP_UTILS_LOGI(
            "\n\t{Device config}[full]"
            "\n\t\t-> [reset_gpio_num]: %d"
            "\n\t\t-> [rgb_ele_order]: %d"
            "\n\t\t-> [bits_per_pixel]: %d"
            "\n\t\t-> {flags}"
            "\n\t\t\t-> [reset_active_high]: %d"
            , config.reset_gpio_num
            , config.rgb_ele_order
            , config.bits_per_pixel
            , config.flags.reset_active_high
        );
    } else {
        auto &config = std::get<DevicePartialConfig>(device);
        ESP_UTILS_LOGI(
            "\n\t{Device config}[partial]"
            "\n\t\t-> [reset_gpio_num]: %d"
            "\n\t\t-> [rgb_ele_order]: %d"
            "\n\t\t-> [bits_per_pixel]: %d"
            "\n\t\t-> [flags_reset_active_high]: %d"
            , config.reset_gpio_num
            , config.rgb_ele_order
            , config.bits_per_pixel
            , config.flags_reset_active_high
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

void LCD::Config::printVendorConfig() const
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (std::holds_alternative<VendorFullConfig>(vendor)) {
        auto &config = std::get<VendorFullConfig>(vendor);
        ESP_UTILS_LOGI(
            "\n\t{Vendor config}[full]"
            "\n\t\t-> [init_cmds]: %p"
            "\n\t\t-> [init_cmds_size]: %d"
#if SOC_LCD_RGB_SUPPORTED
            "\n\t\t-> [rgb_config]: %p"
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
            "\n\t\t-> [mipi_config]"
            "\n\t\t\t-> [lane_num]: %d"
            "\n\t\t\t-> [dsi_bus]: %p"
            "\n\t\t\t-> [dpi_config]: %p"
#endif // SOC_MIPI_DSI_SUPPORTED
            , config.init_cmds
            , config.init_cmds_size
#if SOC_LCD_RGB_SUPPORTED
            , config.rgb_config
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
            , config.mipi_config.lane_num
            , config.mipi_config.dsi_bus
            , config.mipi_config.dpi_config
#endif // SOC_MIPI_DSI_SUPPORTED
        );
        ESP_UTILS_LOGI(
            "\n\t\t-> {flags}"
            "\n\t\t\t-> [mirror_by_cmd]: %d"
            "\n\t\t\t-> [enable_io_multiplex]: %d"
            "\n\t\t\t-> [use_spi_interface]: %d"
            "\n\t\t\t-> [use_qspi_interface]: %d"
            "\n\t\t\t-> [use_rgb_interface]: %d"
            "\n\t\t\t-> [use_mipi_interface]: %d"
            , config.flags.mirror_by_cmd
            , config.flags.enable_io_multiplex
            , config.flags.use_spi_interface
            , config.flags.use_qspi_interface
            , config.flags.use_rgb_interface
            , config.flags.use_mipi_interface
        );
    } else {
        auto &config = std::get<VendorPartialConfig>(vendor);
        ESP_UTILS_LOGI(
            "\n\t{Vendor config}[partial]"
            "\n\t\t-> [init_cmds]: %p"
            "\n\t\t-> [init_cmds_size]: %d"
            "\n\t\t-> [flags_mirror_by_cmd]: %d"
            "\n\t\t-> [flags_enable_io_multiplex]: %d"
            , config.init_cmds
            , config.init_cmds_size
            , config.flags_mirror_by_cmd
            , config.flags_enable_io_multiplex
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool LCD::configVendorCommands(const esp_panel_lcd_vendor_init_cmd_t init_cmd[], uint32_t init_cmd_size)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), false, "Invalid bus");
#if SOC_LCD_RGB_SUPPORTED
    ESP_UTILS_CHECK_FALSE_RETURN(
        (getBus()->getBasicAttributes().type != ESP_PANEL_BUS_TYPE_RGB) ||
        static_cast<BusRGB *>(getBus())->getConfig().control_panel.has_value(), false,
        "Doesn't support the single \"RGB\" bus"
    );
#endif // SOC_LCD_RGB_SUPPORTED

    ESP_UTILS_LOGD("Param: init_cmd(@%p), init_cmd_size(%d)", init_cmd, static_cast<int>(init_cmd_size));
    ESP_UTILS_CHECK_FALSE_RETURN((init_cmd == nullptr) || (init_cmd_size > 0), false, "Invalid arguments");

    auto &vendor_config = getVendorFullConfig();
    vendor_config.init_cmds = init_cmd;
    vendor_config.init_cmds_size = init_cmd_size;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configMirrorByCommand(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), false, "Invalid bus");
#if SOC_LCD_RGB_SUPPORTED
    ESP_UTILS_CHECK_FALSE_RETURN(
        (getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) &&
        static_cast<BusRGB *>(getBus())->getConfig().control_panel.has_value(), false,
        "Only valid for the \"3-wire SPI + RGB\" bus"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    getVendorFullConfig().flags.mirror_by_cmd = en;
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "This function is not supported");
#endif // SOC_LCD_RGB_SUPPORTED

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configEnableIO_Multiplex(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), false, "Invalid bus");
#if SOC_LCD_RGB_SUPPORTED
    ESP_UTILS_CHECK_FALSE_RETURN(
        (getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) &&
        static_cast<BusRGB *>(getBus())->getConfig().control_panel.has_value(), false,
        "Only valid for the \"3-wire SPI + RGB\" bus"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    getVendorFullConfig().flags.enable_io_multiplex = en;
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "This function is not supported");
#endif // SOC_LCD_RGB_SUPPORTED

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configColorRGB_Order(bool reverse_order)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");
    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), false, "Invalid bus");
    ESP_UTILS_LOGD("Param: reverse_order(%d)", reverse_order);
    if (reverse_order) {
        getDeviceFullConfig().rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
    } else {
        getDeviceFullConfig().rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::configResetActiveLevel(int level)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    ESP_UTILS_LOGD("Param: level(%d)", level);
    getDeviceFullConfig().flags.reset_active_high = level;

    return true;
}

bool LCD::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "Already begun");
    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), false, "Invalid bus");

    // Initialize the LCD if not initialized
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    /* Initialize refresh panel */
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_init(refresh_panel), false, "Init panel failed");
    ESP_UTILS_LOGD("Refresh panel(@%p) initialized", refresh_panel);

    auto bus_type = getBus()->getBasicAttributes().type;
    /* If the panel is reset, goto end directly */
    if (isOverState(State::RESET)) {
        goto end;
    }

    /* For non-RGB bus, create Semaphore for using API `drawBitmapWaitUntilFinish()` */
    if ((bus_type != ESP_PANEL_BUS_TYPE_RGB) && (_interruption.draw_bitmap_finish_sem == nullptr)) {
        _interruption.on_draw_bitmap_finish_sem_buffer = utils::make_shared<StaticSemaphore_t>();
        ESP_UTILS_CHECK_NULL_RETURN(
            _interruption.on_draw_bitmap_finish_sem_buffer, false, "Create draw bitmap finish semaphore failed"
        );
        _interruption.draw_bitmap_finish_sem =
            xSemaphoreCreateBinaryStatic(_interruption.on_draw_bitmap_finish_sem_buffer.get());
    }

    /*  Register callback for different bus */
    _interruption.data.lcd_ptr = this;
    switch (bus_type) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB: {
        auto rgb_config = getBusRGB_RefreshPanelFullConfig();
        ESP_UTILS_CHECK_NULL_RETURN(rgb_config, false, "Invalid RGB config");

        esp_lcd_rgb_panel_event_callbacks_t rgb_event_cb = {};
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
        rgb_event_cb.on_frame_buf_complete = (esp_lcd_rgb_panel_frame_buf_complete_cb_t)onRefreshFinish;
#else
        if (rgb_config->bounce_buffer_size_px == 0) {
            // When bounce buffer is disabled, use `on_vsync` callback to notify draw bitmap finish
            rgb_event_cb.on_vsync = (esp_lcd_rgb_panel_vsync_cb_t)onRefreshFinish;
        } else {
            // When bounce buffer is enabled, use `on_bounce_frame_finish` callback to notify draw bitmap finish
            rgb_event_cb.on_bounce_frame_finish = (esp_lcd_rgb_panel_bounce_buf_finish_cb_t)onRefreshFinish;
        }
#endif
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_rgb_panel_register_event_callbacks(refresh_panel, &rgb_event_cb, &_interruption.data), false,
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
            esp_lcd_dpi_panel_register_event_callbacks(refresh_panel, &dpi_event_cb, &_interruption.data), false,
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
            esp_lcd_panel_io_register_event_callbacks(getBus()->getControlPanelHandle(), &io_cb, &_interruption.data),
            false, "Register control panel event callback failed"
        );
        break;
    }

end:
    setState(State::BEGIN);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::reset()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");

    // For RGB bus, if the flag `enable_io_multiplex` is enabled, when the panel is not begun, ignore reset
    if ((getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) && !isOverState(State::BEGIN) &&
            getConfig().getVendorFullConfig()->flags.enable_io_multiplex) {
        ESP_UTILS_LOGW("Ignore `reset()` before `begin()` when using RGB bus with flag `enable_io_multiplex` enabled");
        goto end;
    }

    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_reset(refresh_panel), false, "Reset panel failed");
    ESP_UTILS_LOGD("Refresh panel(@%p) reset", refresh_panel);

end:
    // If not begun, only reset the panel, not reset the state
    if (isOverState(State::BEGIN)) {
        setState(State::RESET);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::del()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (refresh_panel != nullptr) {
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_panel_del(refresh_panel), false, "Delete refresh panel(@%p) failed", refresh_panel
        );
        ESP_UTILS_LOGD("Refresh panel(@%p) deleted", refresh_panel);
        refresh_panel = nullptr;
    }

    _transformation = {};
    _interruption = {};

    setState(State::DEINIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::drawBitmap(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::BEGIN), false, "Not begun");

    ESP_UTILS_LOGD(
        "Param: x_start(%d), y_start(%d), width(%d), height(%d), color_data(@%p)", x_start, y_start, width, height,
        color_data
    );
    auto x_coord_align = getBasicAttributes().basic_bus_spec.x_coord_align;
    auto y_coord_align = getBasicAttributes().basic_bus_spec.y_coord_align;
    if ((x_start & (x_coord_align - 1)) != 0) {
        ESP_UTILS_LOGW("x_start is not aligned to align(%d)", x_coord_align);
    } else if ((width & (x_coord_align - 1)) != 0) {
        ESP_UTILS_LOGW("width is not aligned to align(%d)", x_coord_align);
    }
    if ((y_start & (y_coord_align - 1)) != 0) {
        ESP_UTILS_LOGW("y_start is not aligned to align(%d)", y_coord_align);
    } else if ((height & (y_coord_align - 1)) != 0) {
        ESP_UTILS_LOGW("height is not aligned to align(%d)", y_coord_align);
    }

    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_draw_bitmap(refresh_panel, x_start, y_start, x_start + width, y_start + height, color_data),
        false, "Draw bitmap failed"
    );

    // For RGB bus, since `drawBitmap()` uses `memcpy()` instead of DMA operation, doesn't need to wait for finish
    if ((getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) &&
            (_interruption.on_draw_bitmap_finish != nullptr)) {
        _interruption.on_draw_bitmap_finish(_interruption.data.user_data);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::drawBitmapWaitUntilFinish(
    uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, const uint8_t *color_data, int timeout_ms
)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::BEGIN), false, "Not begun");

    ESP_UTILS_LOGD("Param: timeout_ms(%d)", timeout_ms);
    ESP_UTILS_CHECK_FALSE_RETURN(drawBitmap(x_start, y_start, width, height, color_data), false, "Draw bitmap failed");

    // For RGB bus, since `drawBitmap()` uses `memcpy()` instead of DMA operation, doesn't need to wait for finish
    // For other bus which uses DMA operation, wait for the drawing to finish
    if (getBus()->getBasicAttributes().type != ESP_PANEL_BUS_TYPE_RGB) {
        /* Wait for the semaphore to be given by the callback function */
        BaseType_t timeout_tick = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
        ESP_UTILS_CHECK_FALSE_RETURN(
            xSemaphoreTake(_interruption.draw_bitmap_finish_sem, timeout_tick) == pdTRUE, false,
            "Draw bitmap wait for finish timeout"
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::mirrorX(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_MIRROR_X), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_mirror(refresh_panel, en, _transformation.mirror_y), false, "Mirror X failed"
    );
    _transformation.mirror_x = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::mirrorY(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_MIRROR_Y), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_mirror(refresh_panel, _transformation.mirror_x, en), false, "Mirror X failed"
    );
    _transformation.mirror_y = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::swapXY(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_SWAP_XY), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_swap_xy(refresh_panel, en), false, "Swap XY failed");
    _transformation.swap_xy = en;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::setGapX(uint16_t gap)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_GAP), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: gap(%d)", static_cast<int>(gap));
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_set_gap(refresh_panel, gap, _transformation.gap_y), false, "Set X gap failed"
    );
    _transformation.gap_x = gap;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::setGapY(uint16_t gap)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_GAP), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: gap(%d)", gap);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_set_gap(refresh_panel, _transformation.gap_x, gap), false, "Set Y gap failed"
    );
    _transformation.gap_y = gap;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::invertColor(bool en)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_INVERT_COLOR), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: en(%d)", en);
    ESP_UTILS_CHECK_ERROR_RETURN(esp_lcd_panel_invert_color(refresh_panel, en), false, "Invert color failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::setDisplayOnOff(bool enable_on)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(
        isFunctionSupported(BasicBusSpecification::FUNC_DISPLAY_ON_OFF), false, "This function is not supported"
    );

    ESP_UTILS_LOGD("Param: enable_on(%d)", enable_on);
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_panel_disp_on_off(refresh_panel, enable_on), false, "Set display on/off failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::attachDrawBitmapFinishCallback(FunctionDrawBitmapFinishCallback callback, void *user_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");

    ESP_UTILS_LOGD("Param: callback(@%p), user_data(@%p)", callback, user_data);
    if ((_interruption.data.user_data != nullptr) && (_interruption.data.user_data != user_data)) {
        ESP_UTILS_LOGW("The previous user_data(@%p) is existed, will overwrite it", _interruption.data.user_data);
    }
    _interruption.data.user_data = user_data;
    _interruption.on_draw_bitmap_finish = callback;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::attachRefreshFinishCallback(FunctionRefreshFinishCallback callback, void *user_data)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");
    auto bus_type = getBus()->getBasicAttributes().type;
    ESP_UTILS_CHECK_FALSE_RETURN(
        (bus_type == ESP_PANEL_BUS_TYPE_RGB) || (bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI), false,
        "Only valid for RGB and MIPI-DSI bus"
    );

    ESP_UTILS_LOGD("Param: callback(@%p), user_data(@%p)", callback, user_data);

    /* Check the callback function and user data placement */
    // For MIPI-DSI bus, when `CONFIG_LCD_DSI_ISR_IRAM_SAFE` is set, the callback function and user data should be
    // placed in SRAM
    // For RGB bus, when `CONFIG_LCD_RGB_ISR_IRAM_SAFE` is set, if the "XIP on PSRAM" function is not enabled, the
    // callback function and user data should be placed in SRAM
#if (SOC_MIPI_DSI_SUPPORTED && defined(CONFIG_LCD_DSI_ISR_IRAM_SAFE)) || \
    (SOC_LCD_RGB_SUPPORTED && defined(CONFIG_LCD_RGB_ISR_IRAM_SAFE) && \
    !(defined(CONFIG_SPIRAM_RODATA) && defined(CONFIG_SPIRAM_FETCH_INSTRUCTIONS)))
    if (bus_type == ESP_PANEL_BUS_TYPE_RGB || bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI) {
        ESP_UTILS_CHECK_FALSE_RETURN(
            esp_ptr_in_iram(reinterpret_cast<const void *>(callback)), false,
            "Callback function should be placed in IRAM, add `IRAM_ATTR` before the function"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(
            esp_ptr_internal(user_data), false, "User data should be placed in SRAM, add `DRAM_ATTR` before the data"
        );
    }
#endif

    if ((_interruption.data.user_data != nullptr) && (_interruption.data.user_data != user_data)) {
        ESP_UTILS_LOGW("The previous user_data(@%p) is existed, will overwrite it", _interruption.data.user_data);
    }
    _interruption.data.user_data = user_data;
    _interruption.on_refresh_finish = callback;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD::colorBarTest(uint16_t width, uint16_t height)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::BEGIN), false, "Not begun");

    int bits_per_piexl = getFrameColorBits();
    ESP_UTILS_LOGD("LCD bits per pixel: %d", bits_per_piexl);
    ESP_UTILS_CHECK_FALSE_RETURN(bits_per_piexl > 0, false, "Invalid color bits");

    ESP_UTILS_LOGD("Param: width(%d), height(%d)", width, height);

    auto y_coord_align = getBasicAttributes().basic_bus_spec.y_coord_align;
    int bytes_per_piexl = bits_per_piexl / 8;
    // Make sure the height is aligned to the `y_coord_align`
    int row_per_bar = (height / bits_per_piexl) & ~(y_coord_align - 1);
    int line_count = 0;
    int res_line_count = 0;

    /* Malloc memory for a single color bar */
    auto single_bar_buf_ptr = utils::make_shared<uint8_t[]>(row_per_bar * width * bytes_per_piexl);
    ESP_UTILS_CHECK_NULL_RETURN(single_bar_buf_ptr, false, "Alloc color buffer failed");

    auto single_bar_buf = single_bar_buf_ptr.get();
    auto bus_type = getBus()->getBasicAttributes().type;
    /* Draw color bar from top left to bottom right, the order is B - G - R */
    for (int j = 0; j < bits_per_piexl; j++) {
        for (int i = 0; i < row_per_bar * width; i++) {
            for (int k = 0; k < bytes_per_piexl; k++) {
                if ((bus_type == ESP_PANEL_BUS_TYPE_SPI) || (bus_type == ESP_PANEL_BUS_TYPE_QSPI)) {
                    // For SPI bus, the data bytes should be swapped since the data is sent by LSB first
                    single_bar_buf[i * bytes_per_piexl + k] = SPI_SWAP_DATA_TX(BIT(j), bits_per_piexl) >> (k * 8);
                } else {
                    single_bar_buf[i * bytes_per_piexl + k] = BIT(j) >> (k * 8);
                }
            }
        }
        line_count += row_per_bar;
        ESP_UTILS_CHECK_FALSE_RETURN(
            drawBitmapWaitUntilFinish(0, j * row_per_bar, width, row_per_bar, single_bar_buf), false,
            "Draw bitmap wait until finish failed"
        );
    }

    /* Fill the rest of the screen with white color */
    res_line_count = height - line_count;
    if (res_line_count > 0) {
        ESP_UTILS_LOGD("Fill the rest lines(%d) with white color", res_line_count);
        memset(single_bar_buf, 0xff, row_per_bar * width * bytes_per_piexl);

        for (; res_line_count > 0; res_line_count -= row_per_bar) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                drawBitmapWaitUntilFinish(
                    0, line_count, width, res_line_count > row_per_bar ? row_per_bar : res_line_count, single_bar_buf
                ), false, "Draw bitmap wait until finish failed"
            );
            line_count += row_per_bar;
        }
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

#if SOC_MIPI_DSI_SUPPORTED
bool LCD::DSI_ColorBarPatternTest(DSI_ColorBarPattern pattern)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::BEGIN), false, "Not begun");
    ESP_UTILS_CHECK_FALSE_RETURN(
        getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_MIPI_DSI, false, "Only valid for MIPI-DSI bus"
    );

    ESP_UTILS_LOGD("Param: pattern(%d)", static_cast<int>(pattern));
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_dpi_panel_set_pattern(refresh_panel, static_cast<mipi_dsi_pattern_type_t>(pattern)), false,
        "Set MIPI DPI pattern failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}
#endif /* SOC_MIPI_DSI_SUPPORTED */

int LCD::getFrameColorBits()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), -1, "Invalid bus");

    int bits_per_pixel = -1;
    switch (getBus()->getBasicAttributes().type) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB: {
        auto rgb_config = getBusRGB_RefreshPanelFullConfig();
        ESP_UTILS_CHECK_NULL_RETURN(rgb_config, -1, "Invalid RGB config");

        bits_per_pixel = rgb_config->bits_per_pixel;
        break;
    }
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI: {
        auto dpi_config = getBusDSI_RefreshPanelFullConfig();
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
            bits_per_pixel = getConfig().getDeviceFullConfig()->bits_per_pixel;
            break;
        }
        break;
    }
#endif /* SOC_MIPI_DSI_SUPPORTED */
    default:
        bits_per_pixel = getConfig().getDeviceFullConfig()->bits_per_pixel;
        break;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return bits_per_pixel;
}

void *LCD::getFrameBufferByIndex(uint8_t index)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::BEGIN), nullptr, "Not begun");

    ESP_UTILS_LOGD("Param: index(%d)", index);
    ESP_UTILS_CHECK_FALSE_RETURN(
        index < FRAME_BUFFER_MAX_NUM, nullptr, "Index out of range(0-%d)", FRAME_BUFFER_MAX_NUM - 1
    );

    auto bus_type = getBus()->getBasicAttributes().type;
    void *buffer[FRAME_BUFFER_MAX_NUM] = {};
    switch (bus_type) {
#if SOC_LCD_RGB_SUPPORTED
    case ESP_PANEL_BUS_TYPE_RGB:
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_rgb_panel_get_frame_buffer(refresh_panel, index + 1, &buffer[0], &buffer[1], &buffer[2]), nullptr,
            "Get RGB buffer failed"
        );
        break;
#endif
#if SOC_MIPI_DSI_SUPPORTED
    case ESP_PANEL_BUS_TYPE_MIPI_DSI:
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_dpi_panel_get_frame_buffer(refresh_panel, index + 1, &buffer[0], &buffer[1], &buffer[2]), nullptr,
            "Get MIPI DPI buffer failed"
        );
        break;
#endif
    default:
#if ESP_PANEL_DRIVERS_BUS_ENABLE_FACTORY
        ESP_UTILS_CHECK_FALSE_RETURN(
            false, nullptr, "Bus(%d[%s]) is invalid for this function", bus_type,
            BusFactory::getTypeNameString(bus_type).c_str()
        );
#else
        ESP_UTILS_CHECK_FALSE_RETURN(false, nullptr, "Bus(%d) is invalid for this function", bus_type);
#endif
        break;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return buffer[index];
}

bool LCD::processDeviceOnInit(const BasicBusSpecificationMap &bus_specs)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Should be called before `init()`");

    // Check if the bus is valid and begun
    ESP_UTILS_CHECK_FALSE_RETURN(isBusValid(), false, "Invalid bus");
    ESP_UTILS_CHECK_FALSE_RETURN(getBus()->isOverState(Bus::State::BEGIN), false, "Bus is not begun");

#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
    // Print all bus specifications
    ESP_UTILS_LOGD("Print %s all supported bus specifications", _basic_attributes.name);
    for (const auto &bus_spec : bus_specs) {
#if ESP_PANEL_DRIVERS_BUS_ENABLE_FACTORY
        bus_spec.second.print(BusFactory::getTypeNameString(bus_spec.first));
#else
        bus_spec.second.print(std::to_string(bus_spec.first));
#endif
    }
#endif // ESP_UTILS_LOG_LEVEL_DEBUG

    // Check if the bus is valid for the device and load the bus specification to the device
    auto bus_type = getBus()->getBasicAttributes().type;
    auto bus_specs_it = bus_specs.find(bus_type);
// *INDENT-OFF*
#if ESP_PANEL_DRIVERS_BUS_ENABLE_FACTORY
    ESP_UTILS_CHECK_FALSE_RETURN(
        bus_specs_it != bus_specs.end(), false,
        "Bus type(%d[%s]) is not supported for the device(%s: %s)", bus_type,
        BusFactory::getTypeNameString(bus_type).c_str(), _basic_attributes.name,
        std::accumulate(bus_specs.begin(), bus_specs.end(), std::string(), [](const std::string & a, const auto & b) {
            return a.empty() ? BusFactory::getTypeNameString(b.first) :
                               a + ", " + BusFactory::getTypeNameString(b.first);
        }).c_str()
    );
#else
    ESP_UTILS_CHECK_FALSE_RETURN(
        bus_specs_it != bus_specs.end(), false, "Bus type(%d) is not supported for the device", bus_type
    );
#endif
// *INDENT-ON*
    _basic_attributes.basic_bus_spec = bus_specs_it->second;

    // Convert the device partial configuration to full configuration
    _config.convertPartialToFull();

    auto &bus_spec = _basic_attributes.basic_bus_spec;
// *INDENT-OFF*
    // When using the single RGB bus, the color bits of device configuration will be ignored, so skip the check
    if ((getBus()->getBasicAttributes().type != ESP_PANEL_BUS_TYPE_RGB) ||
        (getBus()->getControlPanelHandle() != nullptr)) {
        // Check if the color bits is valid for the device
        ESP_UTILS_CHECK_FALSE_RETURN(
            std::find(
                bus_spec.color_bits.begin(), bus_spec.color_bits.end(), _config.getDeviceFullConfig()->bits_per_pixel
            ) != bus_spec.color_bits.end(),
            false, "Invalid color bits(%d), supported bits: %s",
            _config.getDeviceFullConfig()->bits_per_pixel, bus_spec.getColorBitsString().c_str()
        );
    }
// *INDENT-ON*

#if SOC_LCD_RGB_SUPPORTED
    // When using RGB bus, if the bus doesn't support the `display_on_off` function, disable it
    if (bus_type == ESP_PANEL_BUS_TYPE_RGB) {
        auto rgb_config = getBusRGB_RefreshPanelFullConfig();
        ESP_UTILS_CHECK_NULL_RETURN(rgb_config, false, "Invalid RGB config");

        auto &vendor_config = getVendorFullConfig();
        if ((rgb_config->disp_gpio_num == -1) && ((getBus()->getControlPanelHandle() == nullptr) ||
                vendor_config.flags.enable_io_multiplex)) {
            ESP_UTILS_LOGD("Not support `display_on_off` function, disable it");
            bus_spec.functions.reset(static_cast<int>(BasicBusSpecification::FUNC_DISPLAY_ON_OFF));
        }
    }
#endif // SOC_LCD_RGB_SUPPORTED

    // Load the vendor configuration from the bus to the device
    ESP_UTILS_LOGD("Load vendor configuration from the bus");
    auto &vendor_config = getVendorFullConfig();
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
        vendor_config.rgb_config = getBusRGB_RefreshPanelFullConfig();
        ESP_UTILS_CHECK_NULL_RETURN(vendor_config.rgb_config, false, "Invalid RGB config");
        break;
#endif
#if SOC_MIPI_DSI_SUPPORTED
    /* Retrieve MIPI DPI configuration from the bus and register it into the vendor configuration */
    case ESP_PANEL_BUS_TYPE_MIPI_DSI: {
        auto bus = static_cast<BusDSI *>(getBus());
        ESP_UTILS_CHECK_FALSE_RETURN(
            std::holds_alternative<BusDSI::HostFullConfig>(bus->getConfig().host), false,
            "MIPI-DSI bus configuration is not full"
        );

        auto &host_config = std::get<BusDSI::HostFullConfig>(bus->getConfig().host);
        vendor_config.flags.use_mipi_interface = 1;
        vendor_config.mipi_config = {
            .lane_num = host_config.num_data_lanes,
            .dsi_bus = bus->getHostHandle(),
            .dpi_config = getBusDSI_RefreshPanelFullConfig(),
        };
        break;
    }
#endif
    default:
#if ESP_PANEL_DRIVERS_BUS_ENABLE_FACTORY
        ESP_UTILS_CHECK_FALSE_RETURN(
            false, false, "Bus(%d[%s]) is invalid for this function", bus_type,
            BusFactory::getTypeNameString(bus_type).c_str()
        );
#else
        ESP_UTILS_CHECK_FALSE_RETURN(false, false, "Bus(%d) is invalid for this function", bus_type);
#endif
        break;
    }

    // Bind the vendor configuration to the device configuration
    auto &device_config = getDeviceFullConfig();
    device_config.vendor_config = &vendor_config;

#if ESP_UTILS_CONF_LOG_LEVEL == ESP_UTILS_LOG_LEVEL_DEBUG
    _basic_attributes.print();
    _config.printVendorConfig();
    _config.printDeviceConfig();
#endif // ESP_UTILS_LOG_LEVEL_DEBUG

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

LCD::DeviceFullConfig &LCD::getDeviceFullConfig()
{
    if (std::holds_alternative<DevicePartialConfig>(_config.device)) {
        _config.convertPartialToFull();
    }

    return std::get<DeviceFullConfig>(_config.device);
}

LCD::VendorFullConfig &LCD::getVendorFullConfig()
{
    if (std::holds_alternative<VendorPartialConfig>(_config.vendor)) {
        _config.convertPartialToFull();
    }

    return std::get<VendorFullConfig>(_config.vendor);
}

#if SOC_LCD_RGB_SUPPORTED
const BusRGB::RefreshPanelFullConfig *LCD::getBusRGB_RefreshPanelFullConfig()
{
    ESP_UTILS_CHECK_NULL_RETURN(getBus(), nullptr, "Invalid bus");
    auto bus_type = getBus()->getBasicAttributes().type;
#if ESP_PANEL_DRIVERS_BUS_ENABLE_FACTORY
    ESP_UTILS_CHECK_FALSE_RETURN(
        bus_type == ESP_PANEL_BUS_TYPE_RGB, nullptr, "Invalid bus type(%d[%s])", bus_type,
        BusFactory::getTypeNameString(bus_type).c_str()
    );
#else
    ESP_UTILS_CHECK_FALSE_RETURN(bus_type == ESP_PANEL_BUS_TYPE_RGB, nullptr, "Invalid bus type(%d)", bus_type);
#endif

    auto &config = static_cast<BusRGB *>(getBus())->getConfig();
    ESP_UTILS_CHECK_FALSE_RETURN(
        std::holds_alternative<BusRGB::RefreshPanelFullConfig>(config.refresh_panel), nullptr, "Config is not full"
    );

    return &std::get<BusRGB::RefreshPanelFullConfig>(config.refresh_panel);
}
#endif

#if SOC_MIPI_DSI_SUPPORTED
const BusDSI::RefreshPanelFullConfig *LCD::getBusDSI_RefreshPanelFullConfig()
{
    ESP_UTILS_CHECK_NULL_RETURN(getBus(), nullptr, "Invalid bus");
    auto bus_type = getBus()->getBasicAttributes().type;
#if ESP_PANEL_DRIVERS_BUS_ENABLE_FACTORY
    ESP_UTILS_CHECK_FALSE_RETURN(
        bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI, nullptr, "Invalid bus type(%d[%s])", bus_type,
        BusFactory::getTypeNameString(bus_type).c_str()
    );
#else
    ESP_UTILS_CHECK_FALSE_RETURN(bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI, nullptr, "Invalid bus type(%d)", bus_type);
#endif
    auto &config = static_cast<BusDSI *>(getBus())->getConfig();
    ESP_UTILS_CHECK_FALSE_RETURN(
        std::holds_alternative<BusDSI::RefreshPanelFullConfig>(config.refresh_panel), nullptr, "Config is not full"
    );

    return &std::get<BusDSI::RefreshPanelFullConfig>(config.refresh_panel);
}
#endif

IRAM_ATTR bool LCD::onDrawBitmapFinish(void *panel_io, void *edata, void *user_ctx)
{
    Interruption::CallbackData *callback_data = (Interruption::CallbackData *)user_ctx;
    if (callback_data == nullptr) {
        return false;
    }

    LCD *lcd_ptr = (LCD *)callback_data->lcd_ptr;
    if (lcd_ptr == nullptr) {
        return false;
    }

    BaseType_t need_yield = pdFALSE;
    if (lcd_ptr->_interruption.on_draw_bitmap_finish != nullptr) {
        need_yield =
            lcd_ptr->_interruption.on_draw_bitmap_finish(lcd_ptr->_interruption.data.user_data) ? pdTRUE : need_yield;
    }
    if (lcd_ptr->_interruption.draw_bitmap_finish_sem != nullptr) {
        xSemaphoreGiveFromISR(lcd_ptr->_interruption.draw_bitmap_finish_sem, &need_yield);
    }

    return (need_yield == pdTRUE);
}

IRAM_ATTR bool LCD::onRefreshFinish(void *panel_io, void *edata, void *user_ctx)
{
    Interruption::CallbackData *callback_data = (Interruption::CallbackData *)user_ctx;
    if (callback_data == nullptr) {
        return false;
    }

    LCD *lcd_ptr = (LCD *)callback_data->lcd_ptr;
    if (lcd_ptr == nullptr) {
        return false;
    }

    BaseType_t need_yield = pdFALSE;
    if (lcd_ptr->_interruption.on_refresh_finish != nullptr) {
        need_yield =
            lcd_ptr->_interruption.on_refresh_finish(lcd_ptr->_interruption.data.user_data) ? pdTRUE : need_yield;
    }

    return (need_yield == pdTRUE);
}

} // namespace esp_panel::drivers
