/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <string>
#include <memory>
#include "esp_panel_types.h"
#include "esp_panel_bus_base.hpp"
#include "esp_panel_bus_dsi.hpp"
#include "esp_panel_bus_i2c.hpp"
#include "esp_panel_bus_qspi.hpp"
#include "esp_panel_bus_rgb.hpp"
#include "esp_panel_bus_spi.hpp"

namespace esp_panel::drivers {

class BusFactory {
public:
    BusFactory() = default;
    ~BusFactory() = default;

    static std::shared_ptr<Bus> create(int type, const void *config);

    static std::string getTypeString(int type);
};

} // namespace esp_panel::drivers
