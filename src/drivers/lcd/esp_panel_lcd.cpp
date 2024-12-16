/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#include "esp_panel_utils.h"
#include "esp_utils_helper.h"
#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

#define DEVICE_CREATOR(controller) \
    [](std::shared_ptr<Bus> bus, const LCD::Config &config) { \
        std::shared_ptr<LCD> device = nullptr; \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = utils::make_shared<LCD_ ## controller>(bus, config)), nullptr, \
            "Create " #controller " failed" \
        ); \
        return device; \
    }
#define ITEM_CONTROLLER(controller) \
    {LCD_ ##controller::BASIC_ATTRIBUTES_DEFAULT.name, DEVICE_CREATOR(controller)}

const std::unordered_map<std::string, LCD_Factory::FunctionDeviceConstructor> LCD_Factory::_name_function_map = {
#if ESP_PANEL_CONF_LCD_ENABLE_AXS15231B
    ITEM_CONTROLLER(AXS15231B),
#endif // CONFIG_ESP_PANEL_LCD_AXS15231B
#if ESP_PANEL_CONF_LCD_ENABLE_EK9716B
    ITEM_CONTROLLER(EK9716B),
#endif // CONFIG_ESP_PANEL_LCD_EK9716B
#if ESP_PANEL_CONF_LCD_ENABLE_EK79007
    ITEM_CONTROLLER(EK79007),
#endif // CONFIG_ESP_PANEL_LCD_EK79007
#if ESP_PANEL_CONF_LCD_ENABLE_GC9A01
    ITEM_CONTROLLER(GC9A01),
#endif // CONFIG_ESP_PANEL_LCD_GC9A01
#if ESP_PANEL_CONF_LCD_ENABLE_GC9B71
    ITEM_CONTROLLER(GC9B71),
#endif // CONFIG_ESP_PANEL_LCD_GC9B71
#if ESP_PANEL_CONF_LCD_ENABLE_GC9503
    ITEM_CONTROLLER(GC9503),
#endif // CONFIG_ESP_PANEL_LCD_GC9503
#if ESP_PANEL_CONF_LCD_ENABLE_HX8399
    ITEM_CONTROLLER(HX8399),
#endif // CONFIG_ESP_PANEL_LCD_HX8399
#if ESP_PANEL_CONF_LCD_ENABLE_ILI9341
    ITEM_CONTROLLER(ILI9341),
#endif // CONFIG_ESP_PANEL_LCD_ILI9341
#if ESP_PANEL_CONF_LCD_ENABLE_ILI9881C
    ITEM_CONTROLLER(ILI9881C),
#endif // CONFIG_ESP_PANEL_LCD_ILI9881C
#if ESP_PANEL_CONF_LCD_ENABLE_JD9165
    ITEM_CONTROLLER(JD9165),
#endif // CONFIG_ESP_PANEL_LCD_JD9165
#if ESP_PANEL_CONF_LCD_ENABLE_JD9365
    ITEM_CONTROLLER(JD9365),
#endif // CONFIG_ESP_PANEL_LCD_JD9365
#if ESP_PANEL_CONF_LCD_ENABLE_NV3022B
    ITEM_CONTROLLER(NV3022B),
#endif // CONFIG_ESP_PANEL_LCD_NV3022B
#if ESP_PANEL_CONF_LCD_ENABLE_SH8601
    ITEM_CONTROLLER(SH8601),
#endif // CONFIG_ESP_PANEL_LCD_SH8601
#if ESP_PANEL_CONF_LCD_ENABLE_SPD2010
    ITEM_CONTROLLER(SPD2010),
#endif // CONFIG_ESP_PANEL_LCD_SPD2010
#if ESP_PANEL_CONF_LCD_ENABLE_ST7262
    ITEM_CONTROLLER(ST7262),
#endif // CONFIG_ESP_PANEL_LCD_ST7262
#if ESP_PANEL_CONF_LCD_ENABLE_ST7701
    ITEM_CONTROLLER(ST7701),
#endif // CONFIG_ESP_PANEL_LCD_ST7701
#if ESP_PANEL_CONF_LCD_ENABLE_ST7703
    ITEM_CONTROLLER(ST7703),
#endif // CONFIG_ESP_PANEL_LCD_ST7703
#if ESP_PANEL_CONF_LCD_ENABLE_ST7789
    ITEM_CONTROLLER(ST7789),
#endif // CONFIG_ESP_PANEL_LCD_ST7789
#if ESP_PANEL_CONF_LCD_ENABLE_ST7796
    ITEM_CONTROLLER(ST7796),
#endif // CONFIG_ESP_PANEL_LCD_ST7796
#if ESP_PANEL_CONF_LCD_ENABLE_ST77903
    ITEM_CONTROLLER(ST77903),
#endif // CONFIG_ESP_PANEL_LCD_ST77903
#if ESP_PANEL_CONF_LCD_ENABLE_ST77916
    ITEM_CONTROLLER(ST77916),
#endif // CONFIG_ESP_PANEL_LCD_ST77916
#if ESP_PANEL_CONF_LCD_ENABLE_ST77922
    ITEM_CONTROLLER(ST77922),
#endif // CONFIG_ESP_PANEL_LCD_ST77922
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
