/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#include "esp_panel_utils.h"
#include "esp_utils_helper.h"
#include "esp_panel_touch.hpp"

namespace esp_panel::drivers {

#define DEVICE_CREATOR(controller) \
    [](Bus *bus, const Touch::Config &config) { \
        std::shared_ptr<Touch> device = nullptr; \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = utils::make_shared<Touch ## controller>(bus, config)), nullptr, \
            "Create " #controller " failed" \
        ); \
        return device; \
    }
#define ITEM_CONTROLLER(controller) \
    {Touch ##controller::BASIC_ATTRIBUTES_DEFAULT.name, DEVICE_CREATOR(controller)}

const std::unordered_map<std::string, TouchFactory::FunctionCreateDevice> TouchFactory::_name_function_map = {
    ITEM_CONTROLLER(AXS15231B),
    ITEM_CONTROLLER(CST816S),
    ITEM_CONTROLLER(FT5x06),
    ITEM_CONTROLLER(GT911),
    ITEM_CONTROLLER(GT1151),
    ITEM_CONTROLLER(SPD2010),
    ITEM_CONTROLLER(ST1633),
    ITEM_CONTROLLER(ST7123),
    ITEM_CONTROLLER(STMPE610),
    ITEM_CONTROLLER(TT21100),
    ITEM_CONTROLLER(XPT2046),
};

std::shared_ptr<Touch> TouchFactory::create(std::string name, Bus *bus, const Touch::Config &config)
{
    ESP_UTILS_LOGD("Param: name(%s), bus(@%p), config(@%p)", name.c_str(), bus, &config);
    ESP_UTILS_CHECK_FALSE_RETURN(bus != nullptr, nullptr, "Invalid bus");

    auto it = _name_function_map.find(name);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _name_function_map.end(), nullptr, "Unknown controller: %s", name.c_str());

    std::shared_ptr<Touch> device = it->second(bus, config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device failed");

    return device;
}

} // namespace esp_panel::drivers
