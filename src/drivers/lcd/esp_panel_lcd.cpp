/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#include "esp_panel_utils.h"
#include "esp_panel_lcd.hpp"

namespace esp_panel::drivers {

#define ITEM_CONTROLLER(controller) \
    {LCD_Factory::Conrtoller::controller, LCD_ ##controller::ATTRIBUTES_DEFAULT.name}
#define CASE_CONTROLLER(controller) \
    case Conrtoller::controller: \
    { \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = esp_utils::make_shared<LCD_ ## controller>(bus, config)), nullptr, "Create %s failed", \
            getControllerNameString(controller).c_str() \
        ); \
        break; \
    } \

const std::map<LCD_Factory::Conrtoller, std::string> LCD_Factory::_controller_name_map = {
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

std::shared_ptr<LCD> LCD_Factory::create(Conrtoller name, std::shared_ptr<Bus> bus, const LCD::Config &config)
{
    ESP_UTILS_LOGD(
        "Param: name(%d[%s]), bus(@%p), config(@%p)", static_cast<int>(name), getControllerNameString(name).c_str(),
        bus, &config
    );
    ESP_UTILS_CHECK_FALSE_RETURN(bus != nullptr, nullptr, "Invalid bus");

    std::shared_ptr<LCD> device = nullptr;
    switch (name) {
#if ESP_PANEL_CONF_LCD_ENABLE_AXS15231B
        CASE_CONTROLLER(AXS15231B);
#endif // ESP_PANEL_CONF_LCD_ENABLE_AXS15231B
#if ESP_PANEL_CONF_LCD_ENABLE_EK9716B
        CASE_CONTROLLER(EK9716B);
#endif // ESP_PANEL_CONF_LCD_ENABLE_EK9716B
#if ESP_PANEL_CONF_LCD_ENABLE_EK79007
        CASE_CONTROLLER(EK79007);
#endif // ESP_PANEL_CONF_LCD_ENABLE_EK79007
#if ESP_PANEL_CONF_LCD_ENABLE_GC9A01
        CASE_CONTROLLER(GC9A01);
#endif // ESP_PANEL_CONF_LCD_ENABLE_GC9A01
#if ESP_PANEL_CONF_LCD_ENABLE_GC9B71
        CASE_CONTROLLER(GC9B71);
#endif // ESP_PANEL_CONF_LCD_ENABLE_GC9B71
#if ESP_PANEL_CONF_LCD_ENABLE_GC9503
        CASE_CONTROLLER(GC9503);
#endif // ESP_PANEL_CONF_LCD_ENABLE_GC9503
#if ESP_PANEL_CONF_LCD_ENABLE_HX8399
        CASE_CONTROLLER(HX8399);
#endif // ESP_PANEL_CONF_LCD_ENABLE_HX8399
#if ESP_PANEL_CONF_LCD_ENABLE_ILI9881C
        CASE_CONTROLLER(ILI9881C);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ILI9881C
#if ESP_PANEL_CONF_LCD_ENABLE_ILI9341
        CASE_CONTROLLER(ILI9341);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ILI9341
#if ESP_PANEL_CONF_LCD_ENABLE_JD9165
        CASE_CONTROLLER(JD9165);
#endif // ESP_PANEL_CONF_LCD_ENABLE_JD9165
#if ESP_PANEL_CONF_LCD_ENABLE_JD9365
        CASE_CONTROLLER(JD9365);
#endif // ESP_PANEL_CONF_LCD_ENABLE_JD9365
#if ESP_PANEL_CONF_LCD_ENABLE_NV3022B
        CASE_CONTROLLER(NV3022B);
#endif // ESP_PANEL_CONF_LCD_ENABLE_NV3022B
#if ESP_PANEL_CONF_LCD_ENABLE_SH8601
        CASE_CONTROLLER(SH8601);
#endif // ESP_PANEL_CONF_LCD_ENABLE_SH8601
#if ESP_PANEL_CONF_LCD_ENABLE_SPD2010
        CASE_CONTROLLER(SPD2010);
#endif // ESP_PANEL_CONF_LCD_ENABLE_SPD2010
#if ESP_PANEL_CONF_LCD_ENABLE_ST7262
        CASE_CONTROLLER(ST7262);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST7262
#if ESP_PANEL_CONF_LCD_ENABLE_ST7701
        CASE_CONTROLLER(ST7701);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST7701
#if ESP_PANEL_CONF_LCD_ENABLE_ST7703
        CASE_CONTROLLER(ST7703);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST7703
#if ESP_PANEL_CONF_LCD_ENABLE_ST7789
        CASE_CONTROLLER(ST7789);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST7789
#if ESP_PANEL_CONF_LCD_ENABLE_ST7796
        CASE_CONTROLLER(ST7796);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST7796
#if ESP_PANEL_CONF_LCD_ENABLE_ST77903
        CASE_CONTROLLER(ST77903);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST77903
#if ESP_PANEL_CONF_LCD_ENABLE_ST77916
        CASE_CONTROLLER(ST77916);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST77916
#if ESP_PANEL_CONF_LCD_ENABLE_ST77922
        CASE_CONTROLLER(ST77922);
#endif // ESP_PANEL_CONF_LCD_ENABLE_ST77922
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(
            false, nullptr, "LCD controller %s is disabled or not supported", getControllerNameString(name).c_str()
        );
    }

    return device;
}

std::string LCD_Factory::getControllerNameString(Conrtoller name)
{
    auto it = _controller_name_map.find(name);
    if (it == _controller_name_map.end()) {
        return "Unknown";
    }

    return it->second;
}

} // namespace esp_panel::drivers
