/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#include "esp_panel_utils.h"
#include "esp_panel_touch.hpp"

namespace esp_panel::drivers {

#define ITEM_CONTROLLER(controller) \
    {TouchFactory::Conrtoller::controller, Touch ##controller::ATTRIBUTES_DEFAULT.name}
#define CASE_CONTROLLER(controller) \
    case Conrtoller::controller: \
        ESP_UTILS_CHECK_EXCEPTION_RETURN( \
            (device = esp_utils::make_shared<Touch ## controller>(bus, config)), nullptr, "Create %s failed", \
            getControllerNameString(controller).c_str() \
        ); \
        break

const std::map<TouchFactory::Conrtoller, std::string> TouchFactory::_controller_name_map = {
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

std::shared_ptr<Touch> TouchFactory::create(Conrtoller name, std::shared_ptr<Bus> bus, const Touch::Config &config)
{
    ESP_UTILS_LOG_TRACE_ENTER();

    ESP_UTILS_LOGD(
        "Param: name(%d[%s]), bus(@%p), config(@%p)", static_cast<int>(name), getControllerNameString(name).c_str(),
        bus, &config
    );
    ESP_UTILS_CHECK_NULL_RETURN(bus, nullptr, "Invalid bus");

    std::shared_ptr<Touch> device = nullptr;
    switch (name) {
#if ESP_PANEL_CONF_TOUCH_ENABLE_AXS15231B
        CASE_CONTROLLER(AXS15231B);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_AXS15231B
#if ESP_PANEL_CONF_TOUCH_ENABLE_CST816S
        CASE_CONTROLLER(CST816S);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_CST816S
#if ESP_PANEL_CONF_TOUCH_ENABLE_FT5x06
        CASE_CONTROLLER(FT5x06);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_FT5x06
#if ESP_PANEL_CONF_TOUCH_ENABLE_GT911
        CASE_CONTROLLER(GT911);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_GT911
#if ESP_PANEL_CONF_TOUCH_ENABLE_GT1151
        CASE_CONTROLLER(GT1151);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_GT1151
#if ESP_PANEL_CONF_TOUCH_ENABLE_SPD2010
        CASE_CONTROLLER(SPD2010);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_SPD2010
#if ESP_PANEL_CONF_TOUCH_ENABLE_ST1633
        CASE_CONTROLLER(ST1633);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_ST1633
#if ESP_PANEL_CONF_TOUCH_ENABLE_ST7123
        CASE_CONTROLLER(ST7123);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_ST7123
#if ESP_PANEL_CONF_TOUCH_ENABLE_STMPE610
        CASE_CONTROLLER(STMPE610);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_STMPE610
#if ESP_PANEL_CONF_TOUCH_ENABLE_TT21100
        CASE_CONTROLLER(TT21100);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_TT21100
#if ESP_PANEL_CONF_TOUCH_ENABLE_XPT2046
        CASE_CONTROLLER(XPT2046);
#endif // ESP_PANEL_CONF_TOUCH_ENABLE_XPT2046
    default:
        ESP_UTILS_CHECK_FALSE_RETURN(
            false, nullptr, "Touch controller %s is disabled or not supported", getControllerNameString(name).c_str()
        );
    }

    ESP_UTILS_LOG_TRACE_EXIT();

    return device;
}

std::string TouchFactory::getControllerNameString(Conrtoller name)
{
    auto it = _controller_name_map.find(name);
    if (it == _controller_name_map.end()) {
        return "Unknown";
    }

    return it->second;
}

} // namespace esp_panel::drivers
