/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include "esp_panel_types.h"
#include "esp_panel_backlight_base.hpp"
#include "esp_panel_backlight_custom.hpp"
#include "esp_panel_backlight_pwm_ledc.hpp"
#include "esp_panel_backlight_switch_gpio.hpp"

namespace esp_panel::drivers {

class BacklightFactory {
public:
    enum class Type {
        SwitchGPIO = ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO,
        PWM_LEDC   = ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC,
        Custom     = ESP_PANEL_BACKLIGHT_TYPE_CUSTOM
    };

    BacklightFactory() = default;
    ~BacklightFactory() = default;

    static std::shared_ptr<Backlight> create(Type type, const void *config);

    static std::string getTypeString(Type type);

private:
    static const std::map<Type, std::string> _type_name_map;
};

} // namespace esp_panel::drivers
