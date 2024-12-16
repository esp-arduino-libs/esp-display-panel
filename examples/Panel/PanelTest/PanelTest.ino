/**
 * # Panel Test Example
 *
 * The example demonstrates how to develop built-in or custom development boards using the `ESP_Panel` driver and tests by displaying color bars and printing touch point coordinates.
 *
 * ## How to Use
 *
 * Follow the steps below to configure:
 *
 * 1. For **ESP32_Display_Panel**:
 *
 *     - Follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-drivers) to configure drivers if needed.
 *     - If using a supported development board, follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#using-supported-development-boards) to configure it.
 *     - If using a custom board, follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#using-custom-development-boards) to configure it.
 *
 * 2. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters. For supported
 *    boards, please refter to [Configuring Supported Development Boards](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-supported-development-boards)
 * 3. Verify and upload the example to your ESP board.
 *
 * ## Serial Output
 *
 * ```bash
 * ...
 * Panel test example start
 * Initialize display panel
 * Turn off the backlight
 * Draw color bar from top to bottom, the order is B - G - R
 * Turn on the backlight
 * Panel test example end
 * Touch point(0): x 141, y 168, strength 47
 * Touch point(1): x 165, y 288, strength 45
 * Touch point(2): x 258, y 343, strength 33
 * Touch point(3): x 371, y 317, strength 24
 * ...
 * ```
 *
 * ## Troubleshooting
 *
 * Please check the [FAQ](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>

#define EXAMPLE_LCD_ENABLE_ATTACH_CALLBACK    (0)
#define EXAMPLE_TOUCH_ENABLE_ATTACH_CALLBACK  (0)
#define EXAMPLE_TOUCH_READ_POINTS_NUM         (5)

ESP_Panel *panel = nullptr;
ESP_PanelLcd *lcd = nullptr;
ESP_PanelTouch *touch = nullptr;
ESP_PanelBacklight *backlight = nullptr;

#if EXAMPLE_LCD_ENABLE_ATTACH_CALLBACK
IRAM_ATTR bool onRefreshFinishCallback(void *user_data)
{
    esp_rom_printf("Refresh finish callback\n");

    return false;
}
#endif

#if TEST_TOUCH_ENABLE_ATTACH_CALLBACK && (ESP_PANEL_BOARD_TOUCH_INT_IO >= 0)
IRAM_ATTR bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println("Panel test example start");

    panel = new ESP_Panel();

    Serial.println("Initialize display panel");
    panel->init();
    panel->begin();

    lcd = panel->getLcd();
    touch = panel->getTouch();
    backlight = panel->getBacklight();

    if (backlight != nullptr) {
        Serial.println("Turn off the backlight");
        backlight->off();
    } else {
        Serial.println("Backlight is not available");
    }

    if (lcd != nullptr) {
#if EXAMPLE_LCD_ENABLE_ATTACH_CALLBACK
        lcd->attachRefreshFinishCallback(onRefreshFinishCallback, NULL);
#endif
        Serial.println("Draw color bar from top to bottom, the order is B - G - R");
        lcd->colorBarTest(panel->getLcdWidth(), panel->getLcdHeight());
    } else {
        Serial.println("LCD is not available");
    }

    if (backlight != nullptr) {
        Serial.println("Turn on the backlight");
        backlight->on();
    }

    if ((touch != nullptr) && touch->isInterruptEnabled()) {
#if EXAMPLE_TOUCH_ENABLE_ATTACH_CALLBACK
            touch->attachInterruptCallback(onTouchInterruptCallback, NULL);
#endif
        Serial.println("Reading touch_device point...");
    } else {
        Serial.println("Touch is not available");
        Serial.println("Panel test example end");
    }
}

void loop()
{
    if (touch != nullptr) {
        ESP_PanelTouchPoint point[EXAMPLE_TOUCH_READ_POINTS_NUM];
        int read_touch_result = touch->readPoints(point, EXAMPLE_TOUCH_READ_POINTS_NUM, -1);

        if (read_touch_result > 0) {
            for (int i = 0; i < read_touch_result; i++) {
                Serial.printf("Touch point(%d): x %d, y %d, strength %d\n", i, point[i].x, point[i].y, point[i].strength);
            }
        } else if (read_touch_result < 0) {
            Serial.println("Read touch point failed");
        }
        // Delay for a while to avoid reading too frequently if the interrupt is not enabled
        if (!touch->isInterruptEnabled()) {
            delay(30);
        }
    } else {
        delay(1000);
        Serial.println("IDLE loop");
    }
}
