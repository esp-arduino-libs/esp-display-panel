/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#include "esp_panel_utils.h"
#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

#define CREATE_FUNCTION(controller) \
    [](std::shared_ptr<Bus> bus, const LCD::Config &config) { \
        std::shared_ptr<LCD> device = nullptr; \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = esp_utils::make_shared<LCD_ ## controller>(bus, config)), nullptr, \
            "Create " #controller " failed" \
        ); \
        return device; \
    }
#define ITEM_CONTROLLER(controller) \
    {LCD_ ##controller::ATTRIBUTES_DEFAULT.name, CREATE_FUNCTION(controller)}

const std::unordered_map<std::string, LCD_Factory::CreateFunction> LCD_Factory::_name_function_map = {
    ITEM_CONTROLLER(AXS15231B),
    ITEM_CONTROLLER(EK9716B),
    ITEM_CONTROLLER(EK79007),
    ITEM_CONTROLLER(GC9A01),
    ITEM_CONTROLLER(GC9B71),
    ITEM_CONTROLLER(GC9503),
    ITEM_CONTROLLER(HX8399),
    ITEM_CONTROLLER(ILI9881C),
    ITEM_CONTROLLER(ILI9341),
    ITEM_CONTROLLER(JD9165),
    ITEM_CONTROLLER(JD9365),
    ITEM_CONTROLLER(NV3022B),
    ITEM_CONTROLLER(SH8601),
    ITEM_CONTROLLER(SPD2010),
    ITEM_CONTROLLER(ST7262),
    ITEM_CONTROLLER(ST7701),
    ITEM_CONTROLLER(ST7703),
    ITEM_CONTROLLER(ST7789),
    ITEM_CONTROLLER(ST7796),
    ITEM_CONTROLLER(ST77903),
    ITEM_CONTROLLER(ST77916),
    ITEM_CONTROLLER(ST77922),
};

std::shared_ptr<LCD> LCD_Factory::create(std::string name, std::shared_ptr<Bus> bus, const LCD::Config &config)
{
    ESP_UTILS_LOGD("Param: name(%s), bus(@%p), config(@%p)", name.c_str(), bus, &config);
    ESP_UTILS_CHECK_FALSE_RETURN(bus != nullptr, nullptr, "Invalid bus");

    auto it = _name_function_map.find(name);
    ESP_UTILS_CHECK_FALSE_RETURN(it != _name_function_map.end(), nullptr, "Unknown controller: %s", name.c_str());

    std::shared_ptr<LCD> device = it->second(bus, config);
    ESP_UTILS_CHECK_NULL_RETURN(device, nullptr, "Create device failed");

    return device;
}

} // namespace esp_panel::drivers
