/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_ST7789
#include "esp_panel_utils.h"
#include "port/esp_lcd_st7789.h"
#include "esp_panel_lcd_st7789.hpp"

namespace esp_panel::drivers {

LCD_ST7789::~LCD_ST7789()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool LCD_ST7789::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");

    /* Load configurations from bus to vendor configurations */
    ESP_UTILS_CHECK_FALSE_RETURN(loadVendorConfigFromBus(), false, "Load vendor config from bus failed");

    /* Create panel handle */
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_st7789(bus->getIO_Handle(), &panel_config, &panel_handle), false, "Create LCD panel failed"
    );
    ESP_UTILS_LOGD("Create LCD panel(@%p)", panel_handle);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_ST7789
