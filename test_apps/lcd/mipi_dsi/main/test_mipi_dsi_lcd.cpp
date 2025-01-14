/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include <memory>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "unity.h"
#include "unity_test_runner.h"
#include "esp_display_panel.hpp"

using namespace std;
using namespace esp_panel::drivers;

/* The following default configurations are for the board 'Espressif: ESP32-P4-Function-EV-Board, EK79007' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_LCD_WIDTH                  (1024)
#define TEST_LCD_HEIGHT                 (600)
#define TEST_LCD_COLOR_BITS             (ESP_PANEL_LCD_COLOR_BITS_RGB888)
#define TEST_LCD_DSI_PHY_LDO_ID         (3)
#define TEST_LCD_DSI_LANE_NUM           (2)
#define TEST_LCD_DSI_LANE_RATE_MBPS     (1000)
#define TEST_LCD_DPI_CLK_MHZ            (52)
#define TEST_LCD_DPI_COLOR_BITS         (ESP_PANEL_LCD_COLOR_BITS_RGB888)
#define TEST_LCD_DPI_HPW                (10)
#define TEST_LCD_DPI_HBP                (160)
#define TEST_LCD_DPI_HFP                (160)
#define TEST_LCD_DPI_VPW                (1)
#define TEST_LCD_DPI_VBP                (23)
#define TEST_LCD_DPI_VFP                (12)
#define TEST_LCD_USE_EXTERNAL_CMD       (0)
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
const esp_panel_lcd_vendor_init_cmd_t lcd_init_cmd[] = {
    // {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    // {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},
    // {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},
    // {0x29, (uint8_t []){0x00}, 0, 120},
    // // or
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),
    ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),
};
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_LCD_PIN_NUM_RST            (27)    // Set to -1 if not used
#define TEST_LCD_PIN_NUM_BK_LIGHT       (26)    // Set to -1 if not used
#define TEST_LCD_BK_LIGHT_ON_LEVEL      (1)
#define TEST_LCD_BK_LIGHT_OFF_LEVEL     !TEST_LCD_BK_LIGHT_ON_LEVEL

/* Enable or disable pattern test */
#define TEST_ENABLE_PATTERN_TEST        (1)
/* Enable or disable printing LCD refresh rate */
#define TEST_ENABLE_PRINT_LCD_FPS       (1)
#define TEST_PRINT_LCD_FPS_PERIOD_MS    (1000)
/* Enable or disable the attachment of a callback function that is called after each bitmap drawing is completed */
#define TEST_ENABLE_ATTACH_CALLBACK     (1)
#define TEST_COLOR_BAR_SHOW_TIME_MS     (5000)

#define delay(ms)                       vTaskDelay(pdMS_TO_TICKS(ms))

static const char *TAG = "test_spi_lcd";

static BacklightPWM_LEDC::Config backlight_config = {
    .ledc_channel = BacklightPWM_LEDC::LEDC_ChannelPartialConfig{
        .io_num = TEST_LCD_PIN_NUM_BK_LIGHT,
        .on_level = TEST_LCD_BK_LIGHT_ON_LEVEL,
    },
};

static BusDSI::Config bus_config = {
    .host = BusDSI::HostPartialConfig{
        .num_data_lanes = TEST_LCD_DSI_LANE_NUM,
        .lane_bit_rate_mbps = TEST_LCD_DSI_LANE_RATE_MBPS,
    },
    .refresh_panel = BusDSI::RefreshPanelPartialConfig{
        .dpi_clock_freq_mhz = TEST_LCD_DPI_CLK_MHZ,
        .bits_per_pixel = TEST_LCD_DPI_COLOR_BITS,
        .h_size = TEST_LCD_WIDTH,
        .v_size = TEST_LCD_HEIGHT,
        .hsync_pulse_width = TEST_LCD_DPI_HPW,
        .hsync_back_porch = TEST_LCD_DPI_HBP,
        .hsync_front_porch = TEST_LCD_DPI_HFP,
        .vsync_pulse_width = TEST_LCD_DPI_VPW,
        .vsync_back_porch = TEST_LCD_DPI_VBP,
        .vsync_front_porch = TEST_LCD_DPI_VFP,
    },
    .phy_ldo = BusDSI::PHY_LDO_PartialConfig{
        .chan_id = TEST_LCD_DSI_PHY_LDO_ID,
    },
};

static LCD::Config lcd_config = {
    .device = LCD::DevicePartialConfig{
        .reset_gpio_num = TEST_LCD_PIN_NUM_RST,
        .bits_per_pixel = TEST_LCD_COLOR_BITS,
    },
#if TEST_LCD_USE_EXTERNAL_CMD
    .vendor = LCD::VendorPartialConfig{
        .init_cmds = lcd_init_cmd,
        .init_cmds_size = sizeof(lcd_init_cmd) / sizeof(lcd_init_cmd[0]),
    },
#endif
};

static shared_ptr<Backlight> init_backlight(BacklightPWM_LEDC::Config *config)
{
#if TEST_LCD_PIN_NUM_BK_LIGHT >= 0
    std::shared_ptr<Backlight> backlight = nullptr;
    if (config != nullptr) {
        ESP_LOGI(TAG, "Initialize backlight with config");
        backlight = make_shared<BacklightPWM_LEDC>(*config);
    } else {
        ESP_LOGI(TAG, "Initialize backlight with individual parameters");
        backlight = make_shared<BacklightPWM_LEDC>(TEST_LCD_PIN_NUM_BK_LIGHT, TEST_LCD_BK_LIGHT_ON_LEVEL);
    }
    TEST_ASSERT_NOT_NULL_MESSAGE(backlight, "Create backlight object failed");

    TEST_ASSERT_TRUE_MESSAGE(backlight->begin(), "Backlight begin failed");
    TEST_ASSERT_TRUE_MESSAGE(backlight->on(), "Backlight on failed");

    return backlight;
#else
    return nullptr;
#endif
}

static shared_ptr<Bus> init_bus(BusDSI::Config *config)
{
    std::shared_ptr<Bus> bus = nullptr;
// *INDENT-OFF*
    if (config != nullptr) {
        ESP_LOGI(TAG, "Initialize bus with config");
        bus = make_shared<BusDSI>(*config);
    } else {
        ESP_LOGI(TAG, "Initialize bus with individual parameters");
        bus = make_shared<BusDSI>(
            /* DSI */
            TEST_LCD_DSI_LANE_NUM, TEST_LCD_DSI_LANE_RATE_MBPS,
            /* DPI */
            TEST_LCD_DPI_CLK_MHZ, TEST_LCD_DPI_COLOR_BITS, TEST_LCD_WIDTH, TEST_LCD_HEIGHT,
            TEST_LCD_DPI_HPW, TEST_LCD_DPI_HBP, TEST_LCD_DPI_HFP,
            TEST_LCD_DPI_VPW, TEST_LCD_DPI_VBP, TEST_LCD_DPI_VFP,
            /* PHY LDO */
            TEST_LCD_DSI_PHY_LDO_ID
        );
    }
// *INDENT-ON*
    TEST_ASSERT_NOT_NULL_MESSAGE(bus, "Create bus object failed");

    TEST_ASSERT_TRUE_MESSAGE(bus->begin(), "Bus begin failed");

    return bus;
}

#if TEST_ENABLE_ATTACH_CALLBACK
IRAM_ATTR static bool onDrawBitmapFinishCallback(void *user_data)
{
    esp_rom_printf("Draw bitmap finish callback\n");

    return false;
}
#endif

#if TEST_ENABLE_PRINT_LCD_FPS
#define TEST_LCD_FPS_COUNT_MAX  (100)
#ifndef millis
#define millis()                (esp_timer_get_time() / 1000)
#endif

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onVsyncEndCallback(void *user_data)
{
    long frame_start_time = *(long *)user_data;
    if (frame_start_time == 0) {
        (*(long *)user_data) = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= TEST_LCD_FPS_COUNT_MAX) {
        fps = TEST_LCD_FPS_COUNT_MAX * 1000 / (millis() - frame_start_time);
        frame_count = 0;
        (*(long *)user_data) = millis();
    }

    return false;
}
#endif

static void run_test(shared_ptr<LCD> lcd, bool use_config)
{
    frame_count = 0;
    fps = 0;
    start_time = 0;

#if TEST_LCD_USE_EXTERNAL_CMD
    if (!use_config) {
        lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd) / sizeof(lcd_init_cmd[0]));
    }
#endif
    TEST_ASSERT_TRUE_MESSAGE(lcd->init(), "LCD init failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->reset(), "LCD reset failed");
    TEST_ASSERT_TRUE_MESSAGE(lcd->begin(), "LCD begin failed");
    if (lcd->getBasicAttributes().basic_bus_spec.isFunctionValid(LCD::BasicBusSpecification::FUNC_DISPLAY_ON_OFF)) {
        TEST_ASSERT_TRUE_MESSAGE(lcd->setDisplayOnOff(true), "LCD display on failed");
    }

#if TEST_ENABLE_PATTERN_TEST
    ESP_LOGI(TAG, "Show MIPI-DSI patterns");
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::BAR_HORIZONTAL), "MIPI DPI bar horizontal pattern test failed"
    );
    delay(1000);
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::BAR_VERTICAL), "MIPI DPI bar vertical pattern test failed"
    );
    delay(1000);
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::BER_VERTICAL), "MIPI DPI ber vertical pattern test failed"
    );
    delay(1000);
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->DSI_ColorBarPatternTest(LCD::DSI_ColorBarPattern::NONE), "MIPI DPI none pattern test failed"
    );
#endif
#if TEST_ENABLE_ATTACH_CALLBACK
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->attachDrawBitmapFinishCallback(onDrawBitmapFinishCallback, nullptr), "Attach callback failed"
    );
#endif
#if TEST_ENABLE_PRINT_LCD_FPS
    TEST_ASSERT_TRUE_MESSAGE(
        lcd->attachRefreshFinishCallback(onVsyncEndCallback, (void *)&start_time), "Attach refresh callback failed"
    );
#endif

    ESP_LOGI(TAG, "Draw color bar from top left to bottom right, the order is B - G - R");
    TEST_ASSERT_TRUE_MESSAGE(lcd->colorBarTest(TEST_LCD_WIDTH, TEST_LCD_HEIGHT), "LCD color bar test failed");

    ESP_LOGI(TAG, "Wait for %d ms to show the color bar", TEST_COLOR_BAR_SHOW_TIME_MS);
#if TEST_ENABLE_PRINT_LCD_FPS
    int i = 0;
    while (i++ < TEST_COLOR_BAR_SHOW_TIME_MS / TEST_PRINT_LCD_FPS_PERIOD_MS) {
        ESP_LOGI(TAG, "FPS: %d", fps);
        vTaskDelay(pdMS_TO_TICKS(TEST_PRINT_LCD_FPS_PERIOD_MS));
    }
#else
    vTaskDelay(pdMS_TO_TICKS(TEST_COLOR_BAR_SHOW_TIME_MS));
#endif
}

template<typename T>
decltype(auto) create_lcd_impl(Bus *bus, const LCD::Config &config)
{
    ESP_LOGI(TAG, "Initialize LCD with config");
    return make_shared<T>(bus, config);
}

template<typename T>
decltype(auto) create_lcd_impl(Bus *bus, std::nullptr_t)
{
    ESP_LOGI(TAG, "Initialize LCD with default parameters");
    return make_shared<T>(bus, TEST_LCD_COLOR_BITS, TEST_LCD_PIN_NUM_RST);
}

#define _CREATE_LCD(name, bus, config) \
    ({ \
        auto lcd = create_lcd_impl<LCD_##name>(bus, config); \
        TEST_ASSERT_NOT_NULL_MESSAGE(lcd, "Create LCD object failed"); \
        lcd; \
    })
#define CREATE_LCD(name, bus, config) _CREATE_LCD(name, bus, config)

#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test LCD (" #name ") to draw color bar", "[lcd][mipi_dsi][" #name "]") \
    { \
        /* 1. Test with individual parameters */ \
        auto backlight = init_backlight(nullptr); \
        auto bus = init_bus(nullptr); \
        auto lcd = CREATE_LCD(name, bus.get(), nullptr); \
        run_test(lcd, false); \
        backlight = nullptr; \
        bus = nullptr; \
        lcd = nullptr; \
        /* 2. Test with config */ \
        backlight = init_backlight(&backlight_config); \
        bus = init_bus(&bus_config); \
        lcd = CREATE_LCD(name, bus.get(), lcd_config); \
        run_test(lcd, true); \
    }

/**
 * Here to create test cases for different LCDs
 */
CREATE_TEST_CASE(EK79007)
CREATE_TEST_CASE(ILI9881C)
