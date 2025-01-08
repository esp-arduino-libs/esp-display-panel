/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"

#include "ESP_PanelLog.h"
#include "bus/RGB.h"
#include "EK9716B.h"

static const char *TAG = "EK9716B_CPP";

ESP_PanelLcd_EK9716B::ESP_PanelLcd_EK9716B(ESP_PanelBus *bus, uint8_t color_bits, int rst_io):
    ESP_PanelLcd(bus, color_bits, rst_io)
{
}

ESP_PanelLcd_EK9716B::ESP_PanelLcd_EK9716B(ESP_PanelBus *bus, const esp_lcd_panel_dev_config_t &panel_config):
    ESP_PanelLcd(bus, panel_config)
{
}

ESP_PanelLcd_EK9716B::~ESP_PanelLcd_EK9716B()
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

bool ESP_PanelLcd_EK9716B::init(void)
{
    ESP_PANEL_ENABLE_TAG_DEBUG_LOG();

    /* Initialize RST pin */
    if (panel_config.reset_gpio_num >= 0) {
        gpio_config_t gpio_conf = {
            .pin_bit_mask = BIT64(panel_config.reset_gpio_num),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_PANEL_CHECK_ERR_RET(gpio_config(&gpio_conf), false, "`Config RST gpio failed");
    }

    /* Load configurations from bus to vendor configurations */
    ESP_PANEL_CHECK_FALSE_RET(loadVendorConfigFromBus(), false, "Load vendor config from bus failed");

    /* Create panel handle */
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_new_rgb_panel(vendor_config.rgb_config, &handle), false, "Create panel failed");

    ESP_LOGD(TAG, "LCD panel @%p created", handle);

    return true;
}

bool ESP_PanelLcd_EK9716B::reset(void)
{
    ESP_PANEL_CHECK_NULL_RET(handle, false, "Invalid handle");

    if (panel_config.reset_gpio_num >= 0) {
        gpio_set_level((gpio_num_t)panel_config.reset_gpio_num, panel_config.flags.reset_active_high);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level((gpio_num_t)panel_config.reset_gpio_num, !panel_config.flags.reset_active_high);
        vTaskDelay(pdMS_TO_TICKS(120));
    }
    ESP_PANEL_CHECK_ERR_RET(esp_lcd_panel_reset(handle), false, "Reset panel failed");

    return true;
}

#endif /* SOC_LCD_RGB_SUPPORTED */
