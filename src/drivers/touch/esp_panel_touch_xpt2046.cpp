/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_touch_conf_internal.h"
#if ESP_PANEL_DRIVERS_TOUCH_ENABLE_XPT2046

#include "esp_panel_utils.h"
#include "esp_panel_touch_xpt2046.hpp"

namespace esp_panel::drivers {

TouchXPT2046::~TouchXPT2046()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool TouchXPT2046::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "Already begun");

    // Initialize the touch if not initialized
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    // Create touch panel
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_touch_new_spi_xpt2046(
            getBus()->getControlPanelHandle(), getConfig().getDeviceFullConfig(), &touch_panel
        ), false, "Create touch panel failed"
    );
    ESP_UTILS_LOGD("Create touch panel(@%p)", touch_panel);

    setState(State::BEGIN);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers

#endif // ESP_PANEL_DRIVERS_TOUCH_ENABLE_XPT2046