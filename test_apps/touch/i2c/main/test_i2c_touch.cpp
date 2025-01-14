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
#include "esp_display_panel.hpp"

using namespace std;
using namespace esp_panel::drivers;

// *INDENT-OFF*
/* The following default configurations are for the board 'Espressif: ESP32_S3_LCD_EV_BOARD_V1_5, GT1151' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your touch spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_TOUCH_ADDRESS          (0)     // Typically set to 0 to use the default address.
                                            // - For touchs with only one address, set to 0
                                            // - For touchs with multiple addresses, set to 0 or the address
                                            //   Like GT911, there are two addresses: 0x5D(default) and 0x14
#define TEST_TOUCH_WIDTH            (480)
#define TEST_TOUCH_HEIGHT           (480)
#define TEST_TOUCH_I2C_FREQ_HZ      (400 * 1000)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_TOUCH_PIN_NUM_I2C_SCL  (48)
#define TEST_TOUCH_PIN_NUM_I2C_SDA  (47)
#define TEST_TOUCH_PIN_NUM_RST      (-1)    // Set to `-1` if not used
                                            // For GT911, the RST pin is also used to configure the I2C address
#define TEST_TOUCH_PIN_NUM_INT      (-1)    // Set to `-1` if not used
                                            // For GT911, the INT pin is also used to configure the I2C address

#define TEST_READ_TOUCH_DELAY_MS    (30)
#define TEST_READ_TOUCH_TIME_MS     (3000)
// *INDENT-ON*

static const char *TAG = "test_i2c_touch";

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

#if TEST_TOUCH_PIN_NUM_INT >= 0
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

static void run_test(shared_ptr<Touch> touch)
{
    touch->init();
    touch->begin();
#if TEST_TOUCH_PIN_NUM_INT >= 0
    touch->attachInterruptCallback(onTouchInterruptCallback, NULL);
#endif

    ESP_LOGI(TAG, "Reading touch point...");

    auto point_ptr = std::make_shared<TouchPoint []>(touch->getBasicAttributes().max_points_num);
    TEST_ASSERT_NOT_NULL_MESSAGE(point_ptr, "Create point array failed");

    uint32_t t = 0;
    auto point = point_ptr.get();
    int touch_cnt = 0;
    while (t++ < TEST_READ_TOUCH_TIME_MS / TEST_READ_TOUCH_DELAY_MS) {
        touch_cnt = touch->readPoints(point, -1, TEST_READ_TOUCH_DELAY_MS);
        if (touch_cnt > 0) {
            for (int i = 0; i < touch_cnt; i++) {
                ESP_LOGI(TAG, "Touch point(%d): x %d, y %d, strength %d", i, point[i].x, point[i].y, point[i].strength);
            }
        } else if (touch_cnt < 0) {
            ESP_LOGE(TAG, "Read touch point failed");
        }
#if TEST_TOUCH_PIN_NUM_INT < 0
        delay(TEST_READ_TOUCH_DELAY_MS);
#endif
    }
}

#define CREATE_TOUCH_BUS(name) \
    ({ \
        ESP_LOGI(TAG, "Create touch bus"); \
        auto bus = make_shared<BusI2C>(TEST_TOUCH_PIN_NUM_I2C_SCL, TEST_TOUCH_PIN_NUM_I2C_SDA, \
                (BusI2C::Config::ControlPanelFullConfig)ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name)); \
        TEST_ASSERT_NOT_NULL_MESSAGE(bus, "Create bus object failed"); \
        bus->configI2C_FreqHz(TEST_TOUCH_I2C_FREQ_HZ); \
        TEST_ASSERT_TRUE_MESSAGE(bus->begin(), "Bus begin failed"); \
        bus; \
    })
#define CREATE_TOUCH(name, bus) \
    ({ \
        ESP_LOGI(TAG, "Create touch device: " #name); \
        auto touch = make_shared<Touch##name>( \
            bus, TEST_TOUCH_WIDTH, TEST_TOUCH_HEIGHT, TEST_TOUCH_PIN_NUM_RST, TEST_TOUCH_PIN_NUM_INT \
        ); \
        TEST_ASSERT_NOT_NULL_MESSAGE(touch, "Create TOUCH object failed"); \
        touch; \
    })
#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test touch (" #name ") to draw color bar", "[i2c_touch][" #name "]") \
    { \
        auto bus = CREATE_TOUCH_BUS(name); \
        auto touch = CREATE_TOUCH(name, bus.get()); \
        run_test(touch); \
    }

/**
 * Here to create test cases for different touchs
 */
CREATE_TEST_CASE(CST816S)
CREATE_TEST_CASE(FT5x06)
CREATE_TEST_CASE(GT1151)
CREATE_TEST_CASE(GT911)
CREATE_TEST_CASE(TT21100)
CREATE_TEST_CASE(ST1633)
CREATE_TEST_CASE(ST7123)
