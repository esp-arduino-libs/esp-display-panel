/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"
#include "esp_panel_utils.h"
#include "esp_utils_helper.h"
#include "esp_panel_bus.hpp"

namespace esp_panel::drivers {

#define DEVICE_CREATOR(type_name)                                                                     \
    [](const BusFactory::Config &config) {                                                            \
        ESP_UTILS_LOG_TRACE_ENTER();                                                                  \
        std::shared_ptr<Bus> device = nullptr;                                                        \
        ESP_UTILS_CHECK_FALSE_RETURN(std::holds_alternative<Bus ##type_name::Config>(config), device, \
            "Bus config is not a " #type_name " config"                                               \
        );                                                                                            \
        ESP_UTILS_CHECK_EXCEPTION_RETURN(                                                             \
            (device = utils::make_shared<Bus ##type_name>(                                        \
                std::get<Bus ##type_name::Config>(config))                                            \
            ), nullptr, "Create " #type_name " failed"                                                \
        );                                                                                            \
        ESP_UTILS_LOG_TRACE_EXIT();                                                                   \
        return device;                                                                                \
    }
#define ITEM_CONTROLLER(type_name)                                                  \
    {                                                                               \
        Bus ##type_name::BASIC_ATTRIBUTES_DEFAULT.type,                             \
        {Bus ##type_name::BASIC_ATTRIBUTES_DEFAULT.name, DEVICE_CREATOR(type_name)} \
    }

const std::unordered_map<int, std::pair<std::string, BusFactory::FunctionDeviceConstructor>>
BusFactory::_type_constructor_map = {
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

std::shared_ptr<Bus> BusFactory::create(const Config &config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD("Param: config(@%p)", &config);

    auto type = getConfigType(config);
    ESP_UTILS_LOGD("Get config type: %d[%s]", type, getTypeNameString(type).c_str());

    auto it = _type_constructor_map.find(type);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _type_constructor_map.end(), nullptr, "Unsupported type: %d", type);

    std::shared_ptr<Bus> device = it->second.second(config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device(%s) failed", it->second.first.c_str());

    ESP_UTILS_LOG_TRACE_EXIT();

    return device;
}

int BusFactory::getConfigType(const Config &config)
{
    if (std::holds_alternative<BusI2C::Config>(config)) {
        return BusI2C::BASIC_ATTRIBUTES_DEFAULT.type;
    } else if (std::holds_alternative<BusSPI::Config>(config)) {
        return BusSPI::BASIC_ATTRIBUTES_DEFAULT.type;
    } else if (std::holds_alternative<BusQSPI::Config>(config)) {
        return BusQSPI::BASIC_ATTRIBUTES_DEFAULT.type;
    }
#if SOC_LCD_RGB_SUPPORTED
    else if (std::holds_alternative<BusRGB::Config>(config)) {
        return BusRGB::BASIC_ATTRIBUTES_DEFAULT.type;
    }
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
    else if (std::holds_alternative<BusDSI::Config>(config)) {
        return BusDSI::BASIC_ATTRIBUTES_DEFAULT.type;
    }
#endif // SOC_MIPI_DSI_SUPPORTED

    return -1;
}

std::string BusFactory::getTypeNameString(int type)
{
    auto it = _type_constructor_map.find(type);
    if (it != _type_constructor_map.end()) {
        return it->second.first;
    }

    return "Unknown";
}

} // namespace esp_panel::drivers
