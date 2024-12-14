/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_panel_backlight_pwm_ledc.hpp"

namespace esp_panel::drivers {

BacklightPWM_LEDC::~BacklightPWM_LEDC()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool BacklightPWM_LEDC::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    ESP_UTILS_CHECK_ERROR_RETURN(ledc_timer_config(&_timer_config), false, "LEDC timer config failed");
    ESP_UTILS_CHECK_ERROR_RETURN(ledc_channel_config(&_channel_config), false, "LEDC channel config failed");

    flags.is_begun = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BacklightPWM_LEDC::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    if (checkIsBegun()) {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 3, 0)
        ESP_UTILS_CHECK_ERROR_RETURN(
            ledc_stop(_channel_config.speed_mode, _channel_config.channel, !_channel_config.flags.output_invert), false,
            "LEDC stop failed"
        );
#else
        ESP_UTILS_CHECK_ERROR_RETURN(
            ledc_timer_pause(_timer_config.speed_mode, _timer_config.timer_num), false, "LEDC timer stop failed"
        );
        _timer_config.deconfigure = true;
        ESP_UTILS_CHECK_ERROR_RETURN(ledc_timer_config(&_timer_config), false, "LEDC timer config failed");
        ESP_UTILS_LOGD("Stop LEDC timer");
#endif // ESP_IDF_VERSION

        flags.is_begun = false;
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool BacklightPWM_LEDC::setBrightness(uint8_t percent)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsBegun(), false, "Not begun");

    ESP_UTILS_LOGD("Param: percent(%d)", percent);
    if (percent > 100) {
        ESP_UTILS_LOGW("Percent out of range, force to 100");
        percent = 100;
    }

    uint32_t duty_cycle = (BIT(_timer_config.duty_resolution) * percent) / 100;
    ledc_channel_t channel = _channel_config.channel;
    ledc_mode_t mode = _channel_config.speed_mode;
    ESP_UTILS_CHECK_ERROR_RETURN(ledc_set_duty(mode, channel, duty_cycle), false, "LEDC set duty failed");
    ESP_UTILS_CHECK_ERROR_RETURN(ledc_update_duty(mode, channel), false, "LEDC update duty failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
