/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "esp_panel_library.hpp"

using namespace std;

/* The following default configurations are for the board 'Espressif: ESP32_S3_BOX_3, ILI9341' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_LCD_WIDTH               (320)
#define TEST_LCD_HEIGHT              (240)
#define TEST_LCD_COLOR_BITS          (16)
#define TEST_LCD_SPI_FREQ_HZ         (40 * 1000 * 1000)
#define TEST_LCD_USE_EXTERNAL_CMD    (1)
#if TEST_LCD_USE_EXTERNAL_CMD
/**
 * LCD initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code.
 *
 * Please uncomment and change the following macro definitions, then use `configVendorCommands()` to pass them in the
 * same format if needed. Otherwise, the LCD driver will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
const esp_lcd_panel_vendor_init_cmd_t lcd_init_cmd[] = {
    // {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    // {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},
    // {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},
    // {0x29, (uint8_t []){0x00}, 0, 120},
    // // or
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC8, {0xFF, 0x93, 0x42}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x0E, 0x0E}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC5, {0xD0}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x02}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB4, {0x02}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE0, {
        0x00, 0x03, 0x08, 0x06, 0x13, 0x09, 0x39, 0x39, 0x48, 0x02, 0x0a, 0x08,
        0x17, 0x17, 0x0F
    }),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE1, {
        0x00, 0x28, 0x29, 0x01, 0x0d, 0x03, 0x3f, 0x33, 0x52, 0x04, 0x0f, 0x0e,
        0x37, 0x38, 0x0F
    }),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {00, 0x1B}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB7, {0x06}),
    ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(100, 0x11),
};
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_LCD_PIN_NUM_SPI_CS      (5)
#define TEST_LCD_PIN_NUM_SPI_DC      (4)
#define TEST_LCD_PIN_NUM_SPI_SCK     (7)
#define TEST_LCD_PIN_NUM_SPI_MOSI    (6)
#define TEST_LCD_PIN_NUM_SPI_MISO    (-1)
#define TEST_LCD_PIN_NUM_RST         (48)    // Set to -1 if not used
#define TEST_LCD_PIN_NUM_BK_LIGHT    (47)    // Set to -1 if not used
#define TEST_LCD_BK_LIGHT_ON_LEVEL   (1)
#define TEST_LCD_BK_LIGHT_OFF_LEVEL !TEST_LCD_BK_LIGHT_ON_LEVEL

/* Enable or disable the attachment of a callback function that is called after each bitmap drawing is completed */
#define TEST_ENABLE_ATTACH_CALLBACK  (1)
#define TEST_COLOR_BAR_SHOW_TIME_MS  (3000)

static const char *TAG = "test_spi_lcd";

static shared_ptr<ESP_PanelBacklight> init_backlight(void)
{
#if TEST_LCD_PIN_NUM_BK_LIGHT >= 0
    ESP_LOGI(TAG, "Initialize backlight control pin and turn it on");
    shared_ptr<ESP_PanelBacklight> backlight = make_shared<ESP_PanelBacklight>(
                TEST_LCD_PIN_NUM_BK_LIGHT, TEST_LCD_BK_LIGHT_ON_LEVEL, true
            );
    TEST_ASSERT_NOT_NULL_MESSAGE(backlight, "Create backlight object failed");

    TEST_ASSERT_TRUE_MESSAGE(backlight->begin(), "Backlight begin failed");
    TEST_ASSERT_TRUE_MESSAGE(backlight->on(), "Backlight on failed");

    return backlight;
#else
    return nullptr;
#endif
}

static shared_ptr<ESP_PanelBusSPI> init_panel_bus(void)
{
    ESP_LOGI(TAG, "Create LCD bus");
    shared_ptr<ESP_PanelBusSPI> panel_bus = make_shared<ESP_PanelBusSPI>(
            TEST_LCD_PIN_NUM_SPI_CS, TEST_LCD_PIN_NUM_SPI_DC, TEST_LCD_PIN_NUM_SPI_SCK,
            TEST_LCD_PIN_NUM_SPI_MOSI, TEST_LCD_PIN_NUM_SPI_MISO);
    TEST_ASSERT_NOT_NULL_MESSAGE(panel_bus, "Create panel bus object failed");

    panel_bus->configSpiFreqHz(TEST_LCD_SPI_FREQ_HZ);
    TEST_ASSERT_TRUE_MESSAGE(panel_bus->begin(), "Panel bus begin failed");

    return panel_bus;
}

#if TEST_ENABLE_ATTACH_CALLBACK
IRAM_ATTR static bool onDrawBitmapFinishCallback(void *user_data)
{
    esp_rom_printf("Draw bitmap finish callback\n");

    return false;
}
#endif

static void run_test(shared_ptr<ESP_PanelLcd> lcd)
{
#if TEST_LCD_USE_EXTERNAL_CMD
    // Configure external initialization commands, should called before `init()`
    lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd) / sizeof(lcd_init_cmd[0]));
#endif
    lcd->configColorRgbOrder(true);
    lcd->configResetActiveLevel(1);
    TEST_ASSERT_TRUE_MESSAGE(lcd->init(), "LCD init failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->reset(), "LCD reset failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->begin(), "LCD begin failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->mirrorX(true), "LCD mirror X failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->mirrorY(true), "LCD mirror Y failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->displayOn(), "LCD display on failed");
#if TEST_ENABLE_ATTACH_CALLBACK
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->attachDrawBitmapFinishCallback(onDrawBitmapFinishCallback, nullptr), "Attach callback failed"
    );
#endif

    ESP_LOGI(TAG, "Draw color bar from top left to bottom right, the order is B - G - R");
    TEST_ASSERT_TRUE_MESSAGE(lcd->colorBarTest(TEST_LCD_WIDTH, TEST_LCD_HEIGHT), "LCD color bar test failed");

    ESP_LOGI(TAG, "Wait for %d ms to show the color bar", TEST_COLOR_BAR_SHOW_TIME_MS);
    vTaskDelay(pdMS_TO_TICKS(TEST_COLOR_BAR_SHOW_TIME_MS));
}

#define CREATE_LCD(name, panel_bus) \
    ({ \
        ESP_LOGI(TAG, "Create LCD device: " #name); \
        shared_ptr<ESP_PanelLcd> lcd = make_shared<ESP_PanelLcd_##name>(panel_bus, TEST_LCD_COLOR_BITS, TEST_LCD_PIN_NUM_RST); \
        TEST_ASSERT_NOT_NULL_MESSAGE(lcd, "Create LCD object failed"); \
        lcd; \
    })
#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test LCD (" #name ") to draw color bar", "[spi_lcd][" #name "]") \
    { \
        shared_ptr<ESP_PanelBacklight> backlight = init_backlight(); \
        shared_ptr<ESP_PanelBusSPI> panel_bus = init_panel_bus(); \
        shared_ptr<ESP_PanelLcd> lcd = CREATE_LCD(name, panel_bus.get()); \
        run_test(lcd); \
    }

/**
 * Here to create test cases for different LCDs
 *
 */
CREATE_TEST_CASE(GC9A01)
CREATE_TEST_CASE(GC9B71)
CREATE_TEST_CASE(ILI9341)
CREATE_TEST_CASE(NV3022B)
CREATE_TEST_CASE(SH8601)
CREATE_TEST_CASE(SPD2010)
CREATE_TEST_CASE(ST7789)
CREATE_TEST_CASE(ST77916)
CREATE_TEST_CASE(ST77922)
