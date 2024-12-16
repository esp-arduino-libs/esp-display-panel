/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_ST77903
#include "soc/soc_caps.h"
#include "esp_panel_utils.h"
#include "port/esp_lcd_st77903_rgb.h"
#include "esp_panel_lcd_st77903.hpp"

namespace esp_panel::drivers {

const LCD::BasicBusSpecificationMap LCD_ST77903::_bus_specifications = initBusSpecifications();

LCD_ST77903::~LCD_ST77903()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool LCD_ST77903::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

#if SOC_LCD_RGB_SUPPORTED
    // Process the device on initialization
    ESP_UTILS_CHECK_FALSE_RETURN(processDeviceOnInit(_bus_specifications), false, "Process device on init failed");

    // Create refresh panel
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_st77903_rgb(
            bus->getControlPanelHandle(), getConfig().getDeviceFullConfig(), &refresh_panel
        ), false, "Create refresh panel failed"
    );
    ESP_UTILS_LOGD("Create refresh panel(@%p)", refresh_panel);
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "RGB is not supported");
#endif // SOC_LCD_RGB_SUPPORTED

    /* Disable control panel if enable `auto_del_panel_io/enable_io_multiplex` flag */
    if (getConfig().getVendorFullConfig()->flags.auto_del_panel_io) {
        bus->disableControlPanelHandle();
    }

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_ST77903
