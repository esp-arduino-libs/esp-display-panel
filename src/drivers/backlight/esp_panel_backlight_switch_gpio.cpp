/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_panel_backlight_switch_gpio.hpp"

namespace esp_panel::drivers {

BacklightSwitchGPIO::~BacklightSwitchGPIO()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BacklightSwitchGPIO::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    ESP_UTILS_CHECK_ERROR_RETURN(gpio_config(&_io_config), false, "GPIO config failed");

    ESP_UTILS_CHECK_ERROR_RETURN(
        gpio_set_level((gpio_num_t)_config.io_num, _config.on_level), false, "GPIO set level failed"
    );

    flags.is_begun = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BacklightSwitchGPIO::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsBegun()) {
        ESP_UTILS_CHECK_ERROR_RETURN(gpio_reset_pin((gpio_num_t)_config.io_num), false, "GPIO reset pin failed");
        flags.is_begun = false;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BacklightSwitchGPIO::setBrightness(uint8_t percent)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: percent(%d)", percent);
    if (percent > 100) {
        ESP_UTILS_LOGW("Percent out of range, force to 100");
        percent = 100;
    }

    bool level = (percent > 0) ? _config.on_level : !_config.on_level;
    ESP_UTILS_CHECK_ERROR_RETURN(gpio_set_level((gpio_num_t)_config.io_num, level), false, "GPIO set level failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
