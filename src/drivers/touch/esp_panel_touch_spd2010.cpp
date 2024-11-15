/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED || ESP_PANEL_CONF_TOUCH_ENABLE_SPD2010
#include "esp_panel_utils.h"
#include "esp_panel_touch_spd2010.hpp"

namespace esp_panel::drivers {

TouchSPD2010::~TouchSPD2010()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool TouchSPD2010::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_touch_new_i2c_spd2010(bus->getIO_Handle(), &touch_config, &touch_handle), false,
        "Create touch panel failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED || ESP_PANEL_CONF_TOUCH_ENABLE_SPD2010
