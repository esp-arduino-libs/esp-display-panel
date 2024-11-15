/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED || ESP_PANEL_CONF_TOUCH_ENABLE_AXS15231B
#include "esp_panel_utils.h"
#include "esp_panel_touch_axs15231b.hpp"

namespace esp_panel::drivers {

TouchAXS15231B::~TouchAXS15231B()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool TouchAXS15231B::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_touch_new_i2c_axs15231b(bus->getIO_Handle(), &touch_config, &touch_handle), false,
        "Create touch panel failed"
    );

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED || ESP_PANEL_CONF_TOUCH_ENABLE_AXS15231B
