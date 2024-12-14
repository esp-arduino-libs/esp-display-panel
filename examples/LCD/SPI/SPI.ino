/**
 * | Supported ESP SoCs | ESP32 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
 * | ------------------ | ----- | -------- | -------- | -------- | -------- | -------- |
 *
 * | Supported LCD Controllers | GC9A01 | GC9B71 | ILI9341 | NV3022B | SH8601 | SPD2010 | ST7789 | ST77916 | ST77922 |
 * | ------------------------- | ------ | ------ | ------- | ------- | ------ | ------- | ------ | ------- | ------- |
 *
 * # SPI LCD Example
 *
 * The example demonstrates how to develop different model LCDs with SPI interface using standalone drivers and test them by displaying color bars.
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
 * SPI LCD example start
 * Initialize backlight control pin and turn it off
 * Create SPI LCD bus
 * Create LCD device
 * Draw color bar from top left to bottom right, the order is B - G - R
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Turn on the backlight
 * SPI LCD example end
 * IDLE loop
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
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Currently, the library supports the following SPI LCDs:
 *      - GC9A01, GC9B71
 *      - ILI9341
 *      - NV3022B
 *      - SH8601
 *      - SPD2010
 *      - ST7789, ST77916, ST77922
 */
#define EXAMPLE_LCD_NAME                ILI9341
#define EXAMPLE_LCD_WIDTH               (320)
#define EXAMPLE_LCD_HEIGHT              (240)
#define EXAMPLE_LCD_COLOR_BITS          (16)
#define EXAMPLE_LCD_SPI_FREQ_HZ         (40 * 1000 * 1000)
#define EXAMPLE_LCD_USE_EXTERNAL_CMD    (1)
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
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
#define EXAMPLE_LCD_PIN_NUM_SPI_CS      (5)
#define EXAMPLE_LCD_PIN_NUM_SPI_DC      (4)
#define EXAMPLE_LCD_PIN_NUM_SPI_SCK     (7)
#define EXAMPLE_LCD_PIN_NUM_SPI_SDA     (6)
#define EXAMPLE_LCD_PIN_NUM_SPI_SDO     (-1)
#define EXAMPLE_LCD_PIN_NUM_RST         (48)    // Set to -1 if not used
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT    (47)    // Set to -1 if not used
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL   (1)
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

/* Enable or disable the attachment of a callback function that is called after each bitmap drawing is completed */
#define EXAMPLE_ENABLE_ATTACH_CALLBACK  (1)

#define _EXAMPLE_LCD_CLASS(name, ...)   ESP_PanelLcd_##name(__VA_ARGS__)
#define EXAMPLE_LCD_CLASS(name, ...)    _EXAMPLE_LCD_CLASS(name, ##__VA_ARGS__)

#if EXAMPLE_ENABLE_ATTACH_CALLBACK
IRAM_ATTR bool onDrawBitmapFinishCallback(void *user_data)
{
    esp_rom_printf("Draw bitmap finish callback\n");

    return false;
}
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println("SPI LCD example start");

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Initialize backlight control pin and turn it off");
    ESP_PanelBacklight *backlight = new ESP_PanelBacklight(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL, true);
    backlight->begin();
    backlight->off();
#endif

    Serial.println("Create SPI LCD bus");
    ESP_PanelBusSPI *panel_bus = new ESP_PanelBusSPI(EXAMPLE_LCD_PIN_NUM_SPI_CS, EXAMPLE_LCD_PIN_NUM_SPI_DC,
                                                       EXAMPLE_LCD_PIN_NUM_SPI_SCK, EXAMPLE_LCD_PIN_NUM_SPI_SDA,
                                                       EXAMPLE_LCD_PIN_NUM_SPI_SDO);
    panel_bus->configSpiFreqHz(EXAMPLE_LCD_SPI_FREQ_HZ);
    panel_bus->begin();

    Serial.println("Create LCD device");
    ESP_PanelLcd *lcd = new EXAMPLE_LCD_CLASS(EXAMPLE_LCD_NAME, panel_bus, EXAMPLE_LCD_COLOR_BITS, EXAMPLE_LCD_PIN_NUM_RST);
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
    // Configure external initialization commands, should called before `init()`
    lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd)/sizeof(lcd_init_cmd[0]));
#endif
    // lcd->configColorRgbOrder(true);
    // lcd->configResetActiveLevel(1);
    lcd->init();
    lcd->reset();
    lcd->begin();
    // lcd->mirrorX(true);
    // lcd->mirrorY(true);
    lcd->displayOn();
#if EXAMPLE_ENABLE_ATTACH_CALLBACK
    /* Attach a callback function which will be called when every bitmap drawing is completed */
    lcd->attachRefreshFinishCallback(onDrawBitmapFinishCallback, NULL);
#endif

    Serial.println("Draw color bar from top left to bottom right, the order is B - G - R");
    lcd->colorBarTest(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT);

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Turn on the backlight");
    backlight->on();
#endif

    Serial.println("SPI LCD example end");
}

void loop()
{
    delay(1000);
    Serial.println("IDLE loop");
}
