/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED || ESP_PANEL_CONF_TOUCH_ENABLE_GT911
#include "esp_panel_utils.h"
#include "drivers/bus/esp_panel_bus_i2c.hpp"
#include "esp_panel_touch_gt911.hpp"

namespace esp_panel::drivers {

TouchGT911::~TouchGT911()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool TouchGT911::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    esp_lcd_touch_io_gt911_config_t tp_gt911_config = {
        .dev_addr = std::static_pointer_cast<BusI2C>(bus)->getI2cAddress(),
    };
    touch_config.driver_data = (void *)&tp_gt911_config;
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_touch_new_i2c_gt911(bus->getIO_Handle(), &touch_config, &touch_handle), false,
        "Create touch panel failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED || ESP_PANEL_CONF_TOUCH_ENABLE_GT911
