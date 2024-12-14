/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <unordered_map>
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
    using CreateFunction = std::shared_ptr<Touch> (*)(std::shared_ptr<Bus> bus, const Touch::Config &config);

    TouchFactory() = default;
    ~TouchFactory() = default;

    static std::shared_ptr<Touch> create(std::string name, std::shared_ptr<Bus> bus, const Touch::Config &config);

private:
    static const std::unordered_map<std::string, CreateFunction> _name_function_map;
};

} // namespace esp_panel::drivers
