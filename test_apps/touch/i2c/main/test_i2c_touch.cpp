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
#include "unity.h"
#include "unity_test_runner.h"
#include "esp_display_panel.hpp"

using namespace std;
using namespace esp_panel::drivers;

// *INDENT-OFF*
/* The following default configurations are for the board 'Espressif: ESP32_S3_BOX_3, GT911' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your touch spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_TOUCH_ADDRESS          (0)     // Typically set to 0 to use the default address.
                                            // - For touchs with only one address, set to 0
                                            // - For touchs with multiple addresses, set to 0 or the address
                                            //   Like GT911, there are two addresses: 0x5D(default) and 0x14
#define TEST_TOUCH_WIDTH            (320)
#define TEST_TOUCH_HEIGHT           (24)
#define TEST_TOUCH_I2C_FREQ_HZ      (400 * 1000)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TEST_TOUCH_PIN_NUM_I2C_SCL  (18)
#define TEST_TOUCH_PIN_NUM_I2C_SDA  (8)
#define TEST_TOUCH_PIN_NUM_I2C_SCL_PULLUP  (1)     // 0/1
#define TEST_TOUCH_PIN_NUM_I2C_SDA_PULLUP  (1)     // 0/1
#define TEST_TOUCH_PIN_NUM_RST      (48)    // Set to `-1` if not used
                                            // For GT911, the RST pin is also used to configure the I2C address
#define TEST_TOUCH_RST_ACTIVE_LEVEL (1)
#define TEST_TOUCH_PIN_NUM_INT      (3)     // Set to `-1` if not used
                                            // For GT911, the INT pin is also used to configure the I2C address

#define TEST_TOUCH_ENABLE_ATTACH_CALLBACK   (1)
#define TEST_TOUCH_READ_DELAY_MS    (30)
#define TEST_TOUCH_READ_TIME_MS     (5000)
// *INDENT-ON*

#define delay(x)     vTaskDelay(pdMS_TO_TICKS(x))

static const char *TAG = "test_i2c_touch";

static const Touch::Config touch_config = {
    .device = Touch::DevicePartialConfig{
        .x_max = TEST_TOUCH_WIDTH,
        .y_max = TEST_TOUCH_HEIGHT,
        .rst_gpio_num = TEST_TOUCH_PIN_NUM_RST,
        .int_gpio_num = TEST_TOUCH_PIN_NUM_INT,
        .levels_reset = TEST_TOUCH_RST_ACTIVE_LEVEL,
    },
};

#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK && (TEST_TOUCH_PIN_NUM_INT >= 0)
IRAM_ATTR static bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

static void run_test(shared_ptr<Touch> touch, bool use_config)
{
    if (!use_config) {
        touch->configResetActiveLevel(TEST_TOUCH_RST_ACTIVE_LEVEL);
    }
    TEST_ASSERT_TRUE_MESSAGE(touch->init(), "Touch init failed");
    TEST_ASSERT_TRUE_MESSAGE(touch->begin(), "Touch begin failed");
#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK && (TEST_TOUCH_PIN_NUM_INT >= 0)
    TEST_ASSERT_TRUE_MESSAGE(
        touch->attachInterruptCallback(onTouchInterruptCallback, NULL), "Touch attach interrupt callback failed"
    );
#endif

    ESP_LOGI(TAG, "Reading touch point...");

    auto point_ptr = std::make_shared<TouchPoint []>(touch->getBasicAttributes().max_points_num);
    TEST_ASSERT_NOT_NULL_MESSAGE(point_ptr, "Create point array failed");

    uint32_t t = 0;
    auto point = point_ptr.get();
    int touch_cnt = 0;
    while (t++ < TEST_TOUCH_READ_TIME_MS / TEST_TOUCH_READ_DELAY_MS) {
        touch_cnt = touch->readPoints(point, -1, TEST_TOUCH_READ_DELAY_MS);
        if (touch_cnt > 0) {
            for (int i = 0; i < touch_cnt; i++) {
                ESP_LOGI(TAG, "Touch point(%d): x %d, y %d, strength %d", i, point[i].x, point[i].y, point[i].strength);
            }
        } else if (touch_cnt < 0) {
            ESP_LOGE(TAG, "Read touch point failed");
        }
#if TEST_TOUCH_PIN_NUM_INT < 0
        delay(TEST_TOUCH_READ_DELAY_MS);
#endif
    }
}

#define CREATE_BUS_WITH_CONFIG(name, config) \
    ({ \
        ESP_LOGI(TAG, "Create bus with config"); \
        auto bus = make_shared<BusI2C>(config); \
        TEST_ASSERT_NOT_NULL_MESSAGE(bus, "Create bus object failed"); \
        TEST_ASSERT_TRUE_MESSAGE(bus->begin(), "Bus begin failed"); \
        bus; \
    })

#define CREATE_BUS(name) \
    ({ \
        ESP_LOGI(TAG, "Create bus with default parameters"); \
        auto bus = make_shared<BusI2C>(TEST_TOUCH_PIN_NUM_I2C_SCL, TEST_TOUCH_PIN_NUM_I2C_SDA, \
            (BusI2C::ControlPanelFullConfig)ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name)); \
        TEST_ASSERT_NOT_NULL_MESSAGE(bus, "Create bus object failed"); \
        TEST_ASSERT_TRUE_MESSAGE(bus->configI2C_FreqHz(TEST_TOUCH_I2C_FREQ_HZ), "Bus config I2C frequency failed"); \
        TEST_ASSERT_TRUE_MESSAGE(bus->begin(), "Bus begin failed"); \
        bus; \
    })

template<typename T>
decltype(auto) create_touch_impl(Bus *bus, const Touch::Config &config)
{
    ESP_LOGI(TAG, "Create touch with config");
    return make_shared<T>(bus, config);
}

template<typename T>
decltype(auto) create_touch_impl(Bus *bus, std::nullptr_t)
{
    ESP_LOGI(TAG, "Create touch with default parameters");
    return make_shared<T>(bus, TEST_TOUCH_WIDTH, TEST_TOUCH_HEIGHT, TEST_TOUCH_PIN_NUM_RST, TEST_TOUCH_PIN_NUM_INT);
}

#define CREATE_TOUCH(name, bus, config) \
    ({ \
        auto touch = create_touch_impl<Touch##name>(bus, config); \
        TEST_ASSERT_NOT_NULL_MESSAGE(touch, "Create touch object failed"); \
        touch; \
    })
#define CREATE_TEST_CASE(name) \
    TEST_CASE("Test touch (" #name ") to read touch point", "[touch][i2c][" #name "]") \
    { \
        /* 1. Test with default parameters */ \
        auto bus = CREATE_BUS(name); \
        auto touch = CREATE_TOUCH(name, bus.get(), nullptr); \
        run_test(touch, false); \
        bus = nullptr; \
        touch = nullptr; \
        gpio_uninstall_isr_service(); \
        /* 2. Test with config */ \
        BusI2C::Config bus_config = { \
            .host = BusI2C::HostPartialConfig{ \
                .sda_io_num = TEST_TOUCH_PIN_NUM_I2C_SDA, \
                .scl_io_num = TEST_TOUCH_PIN_NUM_I2C_SCL, \
                .sda_pullup_en = TEST_TOUCH_PIN_NUM_I2C_SDA_PULLUP, \
                .scl_pullup_en = TEST_TOUCH_PIN_NUM_I2C_SCL_PULLUP, \
            }, \
            .control_panel = (BusI2C::ControlPanelFullConfig)ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(name), \
        }; \
        bus = CREATE_BUS_WITH_CONFIG(name, bus_config); \
        touch = CREATE_TOUCH(name, bus.get(), touch_config); \
        run_test(touch, true); \
        bus = nullptr; \
        touch = nullptr; \
        gpio_uninstall_isr_service(); \
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
