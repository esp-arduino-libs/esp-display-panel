/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include <cstdlib>
#include <cstring>
#include "esp_lcd_panel_io.h"
#include "esp_panel_utils.h"
#include "esp_panel_bus_rgb.hpp"

namespace esp_panel::drivers {

BusRGB::~BusRGB()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BusRGB::configRgbTimingFreqHz(uint32_t hz)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");

    ESP_UTILS_LOGD("Param: hz(%d)", (int)hz);
    _panel_config.timings.pclk_hz = hz;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configRgbTimingPorch(
    uint16_t hpw, uint16_t hbp, uint16_t hfp, uint16_t vpw, uint16_t vbp, uint16_t vfp
)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");

    ESP_UTILS_LOGD("Param: hpw(%d), hbp(%d), hfp(%d), vpw(%d), vbp(%d), vfp(%d)", hpw, hbp, hfp, vpw, vbp, vfp);
    _panel_config.timings.hsync_pulse_width = hpw;
    _panel_config.timings.hsync_back_porch = hbp;
    _panel_config.timings.hsync_front_porch = hfp;
    _panel_config.timings.vsync_pulse_width = vpw;
    _panel_config.timings.vsync_back_porch = vbp;
    _panel_config.timings.vsync_front_porch = vfp;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configRgbTimingFlags(
    bool hsync_idle_low, bool vsync_idle_low, bool de_idle_high, bool pclk_active_neg, bool pclk_idle_high
)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");

    ESP_UTILS_LOGD(
        "Param: hsync_idle_low(%d), vsync_idle_low(%d), de_idle_high(%d), pclk_active_neg(%d), pclk_idle_high(%d)",
        hsync_idle_low, vsync_idle_low, de_idle_high, pclk_active_neg, pclk_idle_high
    );
    _panel_config.timings.flags.hsync_idle_low = hsync_idle_low;
    _panel_config.timings.flags.vsync_idle_low = vsync_idle_low;
    _panel_config.timings.flags.de_idle_high = de_idle_high;
    _panel_config.timings.flags.pclk_active_neg = pclk_active_neg;
    _panel_config.timings.flags.pclk_idle_high = pclk_idle_high;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configRgbFrameBufferNumber(uint8_t num)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");

    ESP_UTILS_LOGD("Param: num(%d)", num);
    _panel_config.num_fbs = num;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configRgbBounceBufferSize(uint32_t size_in_pixel)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");

    ESP_UTILS_LOGD("Param: size_in_pixel(%d)", size_in_pixel);
    _panel_config.bounce_buffer_size_px = size_in_pixel;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configRgbFlagDispActiveLow(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");

    _panel_config.timings.flags.pclk_active_neg = 1;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configSpiLine(
    bool cs_use_expaneer, bool sck_use_expander, bool sda_use_expander, esp_expander::Base *io_expander
)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");
    ESP_UTILS_CHECK_FALSE_RETURN(_flags.use_spi_interface, false, "Not use SPI interface");

    ESP_UTILS_LOGD(
        "Param: cs_use_expaneer(%d), sck_use_expander(%d), sda_use_expander(%d), io_expander(@%p)",
        cs_use_expaneer, sck_use_expander, sda_use_expander, io_expander
    );
    if (cs_use_expaneer) {
        _io_config.line_config.cs_io_type = IO_TYPE_EXPANDER;
        _io_config.line_config.cs_expander_pin =
            (esp_io_expander_pin_num_t)BIT(_io_config.line_config.cs_gpio_num);
    }
    if (sck_use_expander) {
        _io_config.line_config.scl_io_type = IO_TYPE_EXPANDER;
        _io_config.line_config.scl_expander_pin =
            (esp_io_expander_pin_num_t)BIT(_io_config.line_config.scl_gpio_num);
    }
    if (sda_use_expander) {
        _io_config.line_config.sda_io_type = IO_TYPE_EXPANDER;
        _io_config.line_config.sda_expander_pin =
            (esp_io_expander_pin_num_t)BIT(_io_config.line_config.sda_gpio_num);
    }
    if (io_expander != NULL) {
        _io_config.line_config.io_expander = io_expander->getDeviceHandle();
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configSPI_IO_Expander(esp_io_expander_t *expander_handle)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");
    ESP_UTILS_CHECK_FALSE_RETURN(_flags.use_spi_interface, false, "Not use SPI interface");

    ESP_UTILS_LOGD("Param: io_expander(@%p)", expander_handle);
    _io_config.line_config.io_expander = expander_handle;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::configSPI_SCL_ActiveFallingEdge(bool enable)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "This function should be called before `begin()`");
    ESP_UTILS_CHECK_FALSE_RETURN(_flags.use_spi_interface, false, "Not use SPI interface");

    _io_config.spi_mode = enable ? 1 : 0;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIO_HandleValid(), false, "Already begun");

    if (checkUseSPI_Interface()) {
        ESP_UTILS_CHECK_ERROR_RETURN(
            esp_lcd_new_panel_io_3wire_spi(&_io_config, &io_handle), false, "Create panel IO failed"
        );
        ESP_UTILS_LOGD("Create panel IO @%p", io_handle);
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BusRGB::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIO_HandleValid() && checkUseSPI_Interface()) {
        ESP_UTILS_CHECK_FALSE_RETURN(Bus::del(), false, "Delete base bus failed");
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers

#endif // SOC_LCD_RGB_SUPPORTED
