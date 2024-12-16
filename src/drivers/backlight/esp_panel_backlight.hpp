/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <variant>
#include "esp_panel_utils.h"
#include "esp_panel_types.h"
#include "esp_panel_backlight_base.hpp"
#include "esp_panel_backlight_custom.hpp"
#include "esp_panel_backlight_pwm_ledc.hpp"
#include "esp_panel_backlight_switch_gpio.hpp"

namespace esp_panel::drivers {

/**
 * @brief The backlight factory class
 */
class BacklightFactory {
public:
    /**
     * @brief The backlight configuration
     */
    using Config  = std::variant <
                    drivers::BacklightCustom::Config,
                    drivers::BacklightPWM_LEDC::Config,
                    drivers::BacklightSwitchGPIO::Config
                    >;

    /**
     * @brief The constructor function to create the backlight device
     */
    using FunctionDeviceConstructor = std::shared_ptr<Backlight> (*)(const Config &config);

    BacklightFactory() = default;
    ~BacklightFactory() = default;

    /**
     * @brief Create a new backlight device with configuration.
     *
     * @param[in] config The backlight configuration
     *
     * @return The shared pointer to the device if success, otherwise return `nullptr`
     */
    static std::shared_ptr<Backlight> create(const Config &config);

    /**
     * @brief Get the backlight type of the configuration
     *
     * @param[in] config The backlight configuration
     *
     * @return The backlight type (`ESP_PANEL_BACKLIGHT_TYPE_*`) if success, otherwise return `-1`
     */
    static int getConfigType(const Config &config);

    /**
     * @brief Get the backlight type name string
     *
     * @param[in] type The backlight type (`ESP_PANEL_BACKLIGHT_TYPE_*`)
     *
     * @return The backlight type name string if success, otherwise return `Unknown`
     */
    static std::string getTypeNameString(int type);

private:
    static const std::unordered_map<int, std::pair<std::string, FunctionDeviceConstructor>> _type_constructor_map;
};

} // namespace esp_panel::drivers
