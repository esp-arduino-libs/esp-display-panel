/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_panel_backlight_custom.hpp"

namespace esp_panel::drivers {

BacklightCustom::~BacklightCustom()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BacklightCustom::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    ESP_UTILS_CHECK_NULL_RETURN(_config.callback, false, "Invalid callback function");

    flags.is_begun = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BacklightCustom::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsBegun()) {
        flags.is_begun = false;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BacklightCustom::setBrightness(uint8_t percent)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: percent(%d)", percent);
    if (percent > 100) {
        ESP_UTILS_LOGW("Percent out of range, force to 100");
        percent = 100;
    }
    ESP_UTILS_CHECK_FALSE_RETURN(_config.callback(percent, _config.user_data), false, "Run callback failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
