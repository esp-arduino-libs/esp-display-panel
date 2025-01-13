/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_types.h"
#include "esp_panel_utils.h"
#include "esp_panel_backlight_factory.hpp"

namespace esp_panel::drivers {

#define DEVICE_CREATOR(type_name)                                                                           \
    [](const Config &config) {                                                                              \
        ESP_UTILS_LOG_TRACE_ENTER();                                                                        \
        std::shared_ptr<Backlight> device = nullptr;                                                        \
        ESP_UTILS_CHECK_FALSE_RETURN(std::holds_alternative<Backlight ##type_name::Config>(config), device, \
            "Not a " #type_name " config"                                                         \
        );                                                                                                  \
        ESP_UTILS_CHECK_EXCEPTION_RETURN(                                                                   \
            (device = utils::make_shared<Backlight ##type_name>(                                        \
                std::get<Backlight ##type_name::Config>(config))                                            \
            ), nullptr, "Create " #type_name " failed"                                                      \
        );                                                                                                  \
        ESP_UTILS_LOG_TRACE_EXIT();                                                                         \
        return device;                                                                                      \
    }
#define ITEM_CONTROLLER(type_name)                                                        \
    {                                                                                     \
        Backlight ##type_name::BASIC_ATTRIBUTES_DEFAULT.type,                             \
        {Backlight ##type_name::BASIC_ATTRIBUTES_DEFAULT.name, DEVICE_CREATOR(type_name)} \
    }

const std::unordered_map<int, std::pair<std::string, BacklightFactory::FunctionDeviceConstructor>>
BacklightFactory::_type_constructor_map = {
    ITEM_CONTROLLER(SwitchGPIO),
    ITEM_CONTROLLER(PWM_LEDC),
    ITEM_CONTROLLER(Custom),
};

std::shared_ptr<Backlight> BacklightFactory::create(const Config &config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: config(@%p)", &config);

    auto type = getConfigType(config);
    ESP_UTILS_LOGD("Get config type: %d", type);

    auto it = _type_constructor_map.find(type);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _type_constructor_map.end(), nullptr, "Unsupported type: %d", type);

    std::shared_ptr<Backlight> device = it->second.second(config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device(%s) failed", it->second.first.c_str());

    ESP_UTILS_LOG_TRACE_EXIT();

    return device;
}

int BacklightFactory::getConfigType(const Config &config)
{
    if (std::holds_alternative<BacklightSwitchGPIO::Config>(config)) {
        return BacklightSwitchGPIO::BASIC_ATTRIBUTES_DEFAULT.type;
    } else if (std::holds_alternative<BacklightPWM_LEDC::Config>(config)) {
        return BacklightPWM_LEDC::BASIC_ATTRIBUTES_DEFAULT.type;
    } else if (std::holds_alternative<BacklightCustom::Config>(config)) {
        return BacklightCustom::BASIC_ATTRIBUTES_DEFAULT.type;
    }

    return -1;
}

std::string BacklightFactory::getTypeNameString(int type)
{
    auto it = _type_constructor_map.find(type);
    if (it != _type_constructor_map.end()) {
        return it->second.first;
    }

    return "Unknown";
}

} // namespace esp_panel::drivers
