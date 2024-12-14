/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_types.h"
#include "esp_panel_utils.h"
#include "esp_panel_backlight.hpp"

namespace esp_panel::drivers {

#define CREATE_FUNCTION(type_name) \
    [](const Config &config) { \
        std::shared_ptr<Backlight> device = nullptr; \
        ESP_UTILS_CHECK_FALSE_RETURN(std::holds_alternative<Backlight ##type_name::Config>(config), device, \
            "Config is not a " #type_name " config" \
        ); \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = esp_utils::make_shared<Backlight ##type_name>( \
                std::get<Backlight ##type_name::Config>(config)) \
            ), nullptr, "Create " #type_name " failed" \
        ); \
        return device; \
    }
#define ITEM_CONTROLLER(type_name) \
    { \
        Backlight ##type_name::ATTRIBUTES_DEFAULT.type, \
        {Backlight ##type_name::ATTRIBUTES_DEFAULT.name, CREATE_FUNCTION(type_name)} \
    }

const std::unordered_map<int, std::pair<std::string, BacklightFactory::CreateFunction>>
BacklightFactory::_type_name_function_map = {
    ITEM_CONTROLLER(SwitchGPIO),
    ITEM_CONTROLLER(PWM_LEDC),
    ITEM_CONTROLLER(Custom),
};

std::shared_ptr<Backlight> BacklightFactory::create(int type, const Config &config)
{
    ESP_UTILS_LOGD("Param: name(%d), config(@%p)", type, &config);

    auto it = _type_name_function_map.find(type);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _type_name_function_map.end(), nullptr, "Unsupported type: %d", type);

    std::shared_ptr<Backlight> device = it->second.second(config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device(%s) failed", it->second.first.c_str());

    return device;
}

} // namespace esp_panel::drivers
