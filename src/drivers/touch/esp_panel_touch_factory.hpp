/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "drivers/bus/esp_panel_bus.hpp"
#include "esp_panel_touch_conf_internal.h"
#include "esp_panel_touch.hpp"
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

/**
 * @brief Factory class for creating touch screen device instances
 *
 * This class implements the Factory pattern to create different types of touch screen
 * devices based on the device name.
 */
class TouchFactory {
public:
    /**
     * @brief Function pointer type for device creation
     *
     * @param[in] bus Pointer to the bus interface used for communication
     * @param[in] config Configuration parameters for the touch device
     * @return Smart pointer to the created touch device instance, nullptr if creation fails
     */
    using FunctionCreateDevice = std::shared_ptr<Touch> (*)(Bus *bus, const Touch::Config &config);

    /**
     * @brief Create a touch screen device instance
     *
     * @param[in] name Device name identifier (e.g., "GT911", "XPT2046", etc.)
     * @param[in] bus Pointer to the bus interface used for communication
     * @param[in] config Configuration parameters for the touch device
     * @return Smart pointer to the created touch device instance, nullptr if creation fails
     * @note The bus pointer must be valid and initialized before calling this function
     */
    static std::shared_ptr<Touch> create(std::string name, Bus *bus, const Touch::Config &config);

private:
    /**
     * @brief Map associating device names with their creation functions
     *
     * Static map storing the mapping between touch device names and their corresponding
     * creation functions. Each supported touch controller must be registered in this map.
     */
    static const std::unordered_map<std::string, FunctionCreateDevice> _name_function_map;
};

} // namespace esp_panel::drivers