/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"
#include "esp_panel_utils.h"
#include "esp_panel_bus.hpp"

namespace esp_panel::drivers {

#define CREATE_FUNCTION(type_name) \
    [](const void *config) { \
        std::shared_ptr<Bus> device = nullptr; \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = esp_utils::make_shared<Bus ##type_name>( \
                *static_cast<const Bus ##type_name::Config *>(config)) \
            ), nullptr, "Create " #type_name " failed" \
        ); \
        return device; \
    }
#define ITEM_CONTROLLER(type_name) \
    { \
        Bus ##type_name::ATTRIBUTES_DEFAULT.type, \
        {Bus ##type_name::ATTRIBUTES_DEFAULT.name, CREATE_FUNCTION(type_name)} \
    }

const std::unordered_map<int, std::pair<std::string, BusFactory::CreateFunction>>
BusFactory::_type_name_function_map = {
    ITEM_CONTROLLER(I2C),
    ITEM_CONTROLLER(SPI),
    ITEM_CONTROLLER(QSPI),
#if SOC_LCD_RGB_SUPPORTED
    ITEM_CONTROLLER(RGB),
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
    ITEM_CONTROLLER(DSI),
#endif // SOC_MIPI_DSI_SUPPORTED
};

std::shared_ptr<Bus> BusFactory::create(int type, const void *config)
{
    ESP_UTILS_LOGD("Param: name(%d), config(@%p)", type, &config);

    auto it = _type_name_function_map.find(type);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _type_name_function_map.end(), nullptr, "Unsupported type: %d", type);

    std::shared_ptr<Bus> device = it->second.second(config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device(%s) failed", it->second.first.c_str());

    return device;
}

std::string BusFactory::getTypeNameString(int type)
{
    auto it = _type_name_function_map.find(type);
    if (it != _type_name_function_map.end()) {
        return it->second.first;
    }

    return "Unknown";
}

} // namespace esp_panel::drivers
