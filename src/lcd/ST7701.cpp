/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "ESP_PanelLog.h"
#include "ST7701.h"

static const char *TAG = "ST7701_CPP";

ESP_PanelLcd_ST7701::ESP_PanelLcd_ST7701(ESP_PanelBus *bus, uint8_t color_bits, int rst_io):
    ESP_PanelLcd(bus, color_bits, rst_io)
{
}

ESP_PanelLcd_ST7701::ESP_PanelLcd_ST7701(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config):
    ESP_PanelLcd(bus, panel_config)
{
}

ESP_PanelLcd_ST7701::~ESP_PanelLcd_ST7701()
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    if (handle == NULL) {
        goto end;
    }

    if (!del()) {
        ESP_LOGE(TAG, "Delete device failed");
    }

end:
    ESP_LOGD(TAG, "Destroyed");
}

bool ESP_PanelLcd_ST7701::init(void)
{
    ESP_PANEL_CHECK_NULL_RET(bus, false, "Invalid bus");

    /* Load configurations from bus to vendor configurations */
    ESP_PANEL_CHECK_FALSE_RET(loadVendorConfigFromBus(), false, "Load vendor config from bus failed");

    /* Create panel handle */
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_panel_st7701(bus->getPanelIO_Handle(), &panel_config, &handle), false, "Create panel failed");

    // Delete panel io if enable `auto_del_panel_io` or `enable_io_multiplex` flag
    if (((esp_lcd_panel_vendor_config_t *)panel_config.vendor_config)->flags.auto_del_panel_io) {
        ESP_PANEL_CHECK_FALSE_RET(bus->delSkipPanelIO(), false, "Delete panel io failed");
    }

    return true;
}

#endif
