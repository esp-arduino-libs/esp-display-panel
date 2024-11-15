/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include "drivers/bus/esp_panel_bus_base.hpp"
#include "esp_panel_touch_base.hpp"
#include "esp_panel_touch_axs15231b.hpp"
#include "esp_panel_touch_cst816s.hpp"
#include "esp_panel_touch_ft5x06.hpp"
#include "esp_panel_touch_gt911.hpp"
#include "esp_panel_touch_gt1151.hpp"
#include "esp_panel_touch_spd2010.hpp"
#include "esp_panel_touch_st1633.hpp"
#include "esp_panel_touch_st7123.hpp"
#include "esp_panel_touch_stmpe610.hpp"
#include "esp_panel_touch_tt21100.hpp"
#include "esp_panel_touch_xpt2046.hpp"

namespace esp_panel::drivers {

class TouchFactory {
public:
    enum class Conrtoller {
        AXS15231B,
        CST816S,
        FT5x06,
        GT911, GT1151,
        SPD2010,
        ST1633, ST7123,
        STMPE610,
        TT21100,
        XPT2046,
    };

    TouchFactory() = default;
    ~TouchFactory() = default;

    static std::shared_ptr<Touch> create(Conrtoller name, std::shared_ptr<Bus> bus, const Touch::Config &config);

    static std::string getControllerNameString(Conrtoller name);

private:
    static const std::map<Conrtoller, std::string> _controller_name_map;
};

} // namespace esp_panel::drivers
