/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_lcd_conf_internal.h"
#if ESP_PANEL_DRIVERS_LCD_ENABLE_ST7789

#include "esp_panel_utils.h"
#include "port/esp_lcd_st7789.h"
#include "esp_panel_lcd_st7789.hpp"

namespace esp_panel::drivers {

const LCD::BasicBusSpecificationMap LCD_ST7789::_bus_specifications = initBusSpecifications();

LCD_ST7789::~LCD_ST7789()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool LCD_ST7789::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

    // Process the device on initialization
    ESP_UTILS_CHECK_FALSE_RETURN(processDeviceOnInit(_bus_specifications), false, "Process device on init failed");

    // Create refresh panel
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_st7789(
            getBus()->getControlPanelHandle(), getConfig().getDeviceFullConfig(), &refresh_panel
        ), false, "Create refresh panel failed"
    );
    ESP_UTILS_LOGD("Create refresh panel(@%p)", refresh_panel);

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers

#endif // ESP_PANEL_DRIVERS_LCD_ENABLE_ST7789