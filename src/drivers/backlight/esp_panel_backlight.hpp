/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <variant>
#include "esp_panel_types.h"
#include "esp_panel_backlight_base.hpp"
#include "esp_panel_backlight_custom.hpp"
#include "esp_panel_backlight_pwm_ledc.hpp"
#include "esp_panel_backlight_switch_gpio.hpp"

namespace esp_panel::drivers {

class BacklightFactory {
public:
    using Config  = std::variant <
                    drivers::BacklightCustom::Config,
                    drivers::BacklightPWM_LEDC::Config,
                    drivers::BacklightSwitchGPIO::Config
                    >;
    using CreateFunction = std::shared_ptr<Backlight> (*)(const Config &config);

    BacklightFactory() = default;
    ~BacklightFactory() = default;

    static std::shared_ptr<Backlight> create(int type, const Config &config);

private:
    static const std::unordered_map<int, std::pair<std::string, CreateFunction>> _type_name_function_map;
};

} // namespace esp_panel::drivers
