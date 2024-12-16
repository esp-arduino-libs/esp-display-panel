/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_ST7262
#include "soc/soc_caps.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#if SOC_LCD_RGB_SUPPORTED
#include "esp_lcd_panel_rgb.h"
#endif // SOC_LCD_RGB_SUPPORTED
#include "esp_lcd_panel_vendor.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_panel_utils.h"
#include "esp_panel_lcd_st7262.hpp"

namespace esp_panel::drivers {

const LCD::BasicBusSpecificationMap LCD_ST7262::_bus_specifications = initBusSpecifications();

LCD_ST7262::~LCD_ST7262()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool LCD_ST7262::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

#if SOC_LCD_RGB_SUPPORTED
    /* Initialize RST pin */
    auto device_config = getConfig().getDeviceFullConfig();
    if (device_config->reset_gpio_num >= 0) {
        gpio_config_t gpio_conf = {
            .pin_bit_mask = BIT64(device_config->reset_gpio_num),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_UTILS_CHECK_ERROR_RETURN(gpio_config(&gpio_conf), false, "`Config Reset GPIO failed");
    }

    // Process the device on initialization
    ESP_UTILS_CHECK_FALSE_RETURN(processDeviceOnInit(_bus_specifications), false, "Process device on init failed");

    // Create refresh panel
    auto vendor_config = getConfig().getVendorFullConfig();
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_rgb_panel(vendor_config->rgb_config, &refresh_panel), false, "Create refresh panel failed"
    );
    ESP_UTILS_LOGD("Create refresh panel(@%p)", refresh_panel);
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "RGB is not supported");
#endif // SOC_LCD_RGB_SUPPORTED

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool LCD_ST7262::reset()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(isOverState(State::INIT), false, "Not initialized");

    ESP_UTILS_CHECK_FALSE_RETURN(LCD::reset(), false, "Reset base LCD failed");

    auto device_config = getConfig().getDeviceFullConfig();
    if (device_config->reset_gpio_num >= 0) {
        gpio_set_level((gpio_num_t)device_config->reset_gpio_num, device_config->flags.reset_active_high);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level((gpio_num_t)device_config->reset_gpio_num, !device_config->flags.reset_active_high);
        vTaskDelay(pdMS_TO_TICKS(120));
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_ST7262
