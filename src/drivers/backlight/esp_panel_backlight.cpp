/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "esp_panel_types.h"
#include "esp_panel_utils.h"
#include "esp_panel_backlight.hpp"

namespace esp_panel::drivers {

#define ITEM_TYPE(type) \
    {BacklightFactory::Type::type, Backlight ##type::ATTRIBUTES_DEFAULT.name}
#define CASE_TYPE(type) \
    case Type::type: \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (backlight = esp_utils::make_shared<Backlight ##type>( \
                             *static_cast<const Backlight ##type::Config *>(config)) \
            ), nullptr, "Create %s failed", getTypeString(type).c_str() \
        ); \
        break

const std::map<BacklightFactory::Type, std::string> BacklightFactory::_type_name_map = {
    ITEM_TYPE(SwitchGPIO),
    ITEM_TYPE(PWM_LEDC),
    ITEM_TYPE(Custom),
};

std::shared_ptr<Backlight> BacklightFactory::create(Type type, const void *config)
{
    ESP_UTILS_LOGD("Param: type(%d[%s]), config(@%p)", type, getTypeString(type).c_str(), config);
    ESP_UTILS_CHECK_FALSE_RETURN(config != nullptr, nullptr, "Invalid config");

    std::shared_ptr<Backlight> backlight = nullptr;
    switch (type) {
        CASE_TYPE(SwitchGPIO);
        CASE_TYPE(PWM_LEDC);
        CASE_TYPE(Custom);
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(false, nullptr, "Unknown type");
    }

    return backlight;
}

std::string BacklightFactory::getTypeString(Type type)
{
    auto it = _type_name_map.find(type);
    if (it == _type_name_map.end()) {
        return "Unknown";
    }

    return it->second;
}

} // namespace esp_panel::drivers
