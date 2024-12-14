/**
 * | Supported ESP SoCs | ESP32 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
 * | ------------------ | ----- | -------- | -------- | -------- | -------- | -------- |
 *
 * | Supported Touch Controllers | CST816S | FT5x06 | GT911 | GT1151 | ST1633 | ST7123  | TT21100 |
 * | --------------------------- | ------- | ------ | ----- | ------ | ------ | ------- | ------- |
 *
 * # I2C Touch Example
 *
 * The example demonstrates how to develop different model touches with I2C interface using standalone drivers and test them by printing touch point coordinates.
 *
 * ## How to use
 *
 * 1. [Configure drivers](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-drivers) if needed.
 * 2. Modify the macros in the example to match the parameters according to your hardware.
 * 3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-supported-development-boards)
 * 4. Verify and upload the example to your ESP board.
 *
 * ## Serial Output
 *
 * ```
 * ...
 * I2C touch example start
 * Create I2C bus
 * Create touch device
 * I2C touch example end
 * Touch point(0): x 134, y 169, strength 50
 * Touch point(1): x 154, y 301, strength 51
 * Touch point(2): x 245, y 379, strength 30
 * Touch point(3): x 290, y 75, strength 26
 * Touch point(4): x 353, y 391, strength 35
 * ...
 * ```
 *
 * ## Troubleshooting
 *
 * Please check the [FAQ](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
 *
 */

#include <Arduino.h>
#include <esp_panel_library.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your touch spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Currently, the library supports the following I2C touch devices:
 *      - CST816S
 *      - FT5x06
 *      - GT911, GT1151
 *      - TT21100
 *      - ST1633, ST7123
 */
#define EXAMPLE_TOUCH_NAME              GT911
#define EXAMPLE_TOUCH_ADDRESS           (0)     // Typically set to 0 to use the default address.
                                                // - For touchs with only one address, set to 0
                                                // - For touchs with multiple addresses, set to 0 or the address
                                                //   Like GT911, there are two addresses: 0x5D(default) and 0x14
#define EXAMPLE_TOUCH_WIDTH             (480)
#define EXAMPLE_TOUCH_HEIGHT            (480)
#define EXAMPLE_TOUCH_I2C_FREQ_HZ       (400 * 1000)
#define EXAMPLE_TOUCH_READ_POINTS_NUM   (5)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_TOUCH_PIN_NUM_I2C_SCL   (10)
#define EXAMPLE_TOUCH_PIN_NUM_I2C_SDA   (9)
#define EXAMPLE_TOUCH_PIN_NUM_RST       (13)    // Set to `-1` if not used
                                                // For GT911, the RST pin is also used to configure the I2C address
#define EXAMPLE_TOUCH_PIN_NUM_INT       (14)    // Set to `-1` if not used
                                                // For GT911, the INT pin is also used to configure the I2C address

#define _EXAMPLE_TOUCH_CLASS(name, ...) ESP_PanelTouch_##name(__VA_ARGS__)
#define EXAMPLE_TOUCH_CLASS(name, ...)  _EXAMPLE_TOUCH_CLASS(name, ##__VA_ARGS__)

#if EXAMPLE_TOUCH_PIN_NUM_INT >= 0
IRAM_ATTR bool onTouchInterruptCallback(void *user_data)
{
    esp_rom_printf("Touch interrupt callback\n");

    return false;
}
#endif

ESP_PanelTouch *touch = nullptr;

void setup()
{
    Serial.begin(115200);
    Serial.println("I2C touch example start");

    Serial.println("Create I2C bus");
#if EXAMPLE_TOUCH_ADDRESS == 0
    ESP_PanelBusI2C *touch_bus = new ESP_PanelBusI2C(EXAMPLE_TOUCH_PIN_NUM_I2C_SCL, EXAMPLE_TOUCH_PIN_NUM_I2C_SDA,
                                                       ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(EXAMPLE_TOUCH_NAME));
    // Taking GT911 as an example, the following is the code after macro expansion:
    // ESP_PanelBusI2C *touch_bus = new ESP_PanelBusI2C(EXAMPLE_TOUCH_PIN_NUM_I2C_SCL, EXAMPLE_TOUCH_PIN_NUM_I2C_SDA,
    //                                                    ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG());
#else
    ESP_PanelBusI2C *touch_bus = new ESP_PanelBusI2C(EXAMPLE_TOUCH_PIN_NUM_I2C_SCL, EXAMPLE_TOUCH_PIN_NUM_I2C_SDA,
                            ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(EXAMPLE_TOUCH_NAME, EXAMPLE_TOUCH_ADDRESS));
    // Taking GT911 as an example, the following is the code after macro expansion:
    // ESP_PanelBusI2C *touch_bus = new ESP_PanelBusI2C(EXAMPLE_TOUCH_PIN_NUM_I2C_SCL, EXAMPLE_TOUCH_PIN_NUM_I2C_SDA,
    //                                                    ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG_WITH_ADDR(EXAMPLE_TOUCH_ADDRESS));
#endif
    touch_bus->configI2cFreqHz(EXAMPLE_TOUCH_I2C_FREQ_HZ);
    touch_bus->begin();

    Serial.println("Create touch device");
    touch = new EXAMPLE_TOUCH_CLASS(EXAMPLE_TOUCH_NAME, touch_bus, EXAMPLE_TOUCH_WIDTH, EXAMPLE_TOUCH_HEIGHT,
                                    EXAMPLE_TOUCH_PIN_NUM_RST, EXAMPLE_TOUCH_PIN_NUM_INT);
    // Taking GT911 as an example, the following is the code after macro expansion:
    // touch = new ESP_PanelTouch_GT911(touch_bus, EXAMPLE_TOUCH_WIDTH, EXAMPLE_TOUCH_HEIGHT,
    //                                  EXAMPLE_TOUCH_PIN_NUM_RST, EXAMPLE_TOUCH_PIN_NUM_INT);
    touch->init();
    touch->begin();
#if EXAMPLE_TOUCH_PIN_NUM_INT >= 0
    touch->attachInterruptCallback(onTouchInterruptCallback, NULL);
#endif

    Serial.println("I2C touch example end");
}

void loop()
{
    ESP_PanelTouchPoint point[EXAMPLE_TOUCH_READ_POINTS_NUM];
    int read_touch_result = touch->readPoints(point, EXAMPLE_TOUCH_READ_POINTS_NUM, -1);

    if (read_touch_result > 0) {
        for (int i = 0; i < read_touch_result; i++) {
            Serial.printf("Touch point(%d): x %d, y %d, strength %d\n", i, point[i].x, point[i].y, point[i].strength);
        }
    } else if (read_touch_result < 0) {
        Serial.println("Read touch point failed");
    }
#if EXAMPLE_TOUCH_PIN_NUM_INT < 0
    delay(30);
#endif
}
