/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_conf_internal.h"
#if ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_GC9503
#include "soc/soc_caps.h"
#include "esp_panel_utils.h"
#include "port/esp_lcd_gc9503.h"
#include "esp_panel_lcd_gc9503.hpp"

namespace esp_panel::drivers {

LCD_GC9503::~LCD_GC9503()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_EXIT(del(), "Delete failed");

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();
}

bool LCD_GC9503::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");

#if SOC_LCD_RGB_SUPPORTED
    /* Load configurations from bus to vendor configurations */
    ESP_UTILS_CHECK_FALSE_RETURN(loadVendorConfigFromBus(), false, "Load vendor config from bus failed");

    /* Create panel handle */
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_lcd_new_panel_gc9503(bus->getIO_Handle(), &panel_config, &panel_handle), false, "Create LCD panel failed"
    );
    ESP_UTILS_LOGD("Create LCD panel(@%p)", panel_handle);
#else
    ESP_UTILS_CHECK_FALSE_RETURN(false, false, "RGB is not supported");
#endif // SOC_LCD_RGB_SUPPORTED

    /* Disable panel IO if enable `auto_del_panel_io/enable_io_multiplex` flag */
    if ((static_cast<esp_panel_lcd_vendor_config_t *>(panel_config.vendor_config))->flags.auto_del_panel_io) {
        bus->disableIO_Handle();
    }

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel::drivers
#endif // ESP_PANEL_CONF_LCD_COMPILE_DISABLED || ESP_PANEL_CONF_LCD_ENABLE_GC9503
