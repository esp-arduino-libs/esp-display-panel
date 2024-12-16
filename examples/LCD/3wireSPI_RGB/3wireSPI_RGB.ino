/**
 * | Supported ESP SoCs | ESP32-S3 |
 * | ------------------ | -------- |
 *
 * | Supported LCD Controllers | GC9503 | ST7701 |
 * | ------------------------- | ------ | ------ |
 *
 * # 3-wire SPI + RGB LCD Example
 *
 * The example demonstrates how to develop different model LCDs with "3-wire SPI + RGB" interface using standalone drivers and test them by displaying color bars.
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
 * 3-wire SPI + RGB LCD example start
 * Initialize backlight and turn it off
 * Create 3-wire SPI + RGB LCD bus
 * Create LCD device
 * Draw color bar from top left to bottom right, the order is B - G - R
 * Turn on the backlight
 * 3-wire SPI + RGB LCD example end
 * RGB refresh rate: 0
 * RGB refresh rate: 60
 * RGB refresh rate: 60
 * RGB refresh rate: 60
 * ...
 * ```
 *
 * ## Troubleshooting
 *
 * Please check the [FAQ](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>

/* The following default configurations are for the board 'jingcai: ESP32_4848S040C_I_Y_3, ST7701' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Currently, the library supports the following "3-wire SPI + RGB" LCDs:
 *      - GC9503
 *      - ST7701
 */
#define EXAMPLE_LCD_NAME                    ST7701
#define EXAMPLE_LCD_WIDTH                   (480)
#define EXAMPLE_LCD_HEIGHT                  (480)
                                                    // | 8-bit RGB888 | 16-bit RGB565 |
#define EXAMPLE_LCD_COLOR_BITS              (18)    // |      24      |   16/18/24    |
#define EXAMPLE_LCD_RGB_DATA_WIDTH          (16)    // |      8       |      16       |
#define EXAMPLE_LCD_RGB_TIMING_FREQ_HZ      (16 * 1000 * 1000)
#define EXAMPLE_LCD_RGB_TIMING_HPW          (10)
#define EXAMPLE_LCD_RGB_TIMING_HBP          (10)
#define EXAMPLE_LCD_RGB_TIMING_HFP          (20)
#define EXAMPLE_LCD_RGB_TIMING_VPW          (10)
#define EXAMPLE_LCD_RGB_TIMING_VBP          (10)
#define EXAMPLE_LCD_RGB_TIMING_VFP          (10)
#define EXAMPLE_LCD_RGB_BOUNCE_BUFFER_SIZE  (EXAMPLE_LCD_WIDTH * 10)
#define EXAMPLE_LCD_USE_EXTERNAL_CMD        (1)
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
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC2, {0x31, 0x05}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xCD, {0x00}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB0, {0x00, 0x11, 0x18, 0x0E, 0x11, 0x06, 0x07, 0x08, 0x07, 0x22, 0x04, 0x12,
                                                0x0F, 0xAA, 0x31, 0x18}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {0x00, 0x11, 0x19, 0x0E, 0x12, 0x07, 0x08, 0x08, 0x08, 0x22, 0x04, 0x11,
                                                0x11, 0xA9, 0x32, 0x18}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x11}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB0, {0x60}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB1, {0x32}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB2, {0x07}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB3, {0x80}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB5, {0x49}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB7, {0x85}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xB8, {0x21}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x78}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC2, {0x78}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE0, {0x00, 0x1B, 0x02}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE1, {0x08, 0xA0, 0x00, 0x00, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x44, 0x44}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE2, {0x11, 0x11, 0x44, 0x44, 0xED, 0xA0, 0x00, 0x00, 0xEC, 0xA0, 0x00, 0x00}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE3, {0x00, 0x00, 0x11, 0x11}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE4, {0x44, 0x44}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE5, {0x0A, 0xE9, 0xD8, 0xA0, 0x0C, 0xEB, 0xD8, 0xA0, 0x0E, 0xED, 0xD8, 0xA0,
                                                0x10, 0xEF, 0xD8, 0xA0}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE6, {0x00, 0x00, 0x11, 0x11}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE7, {0x44, 0x44}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE8, {0x09, 0xE8, 0xD8, 0xA0, 0x0B, 0xEA, 0xD8, 0xA0, 0x0D, 0xEC, 0xD8, 0xA0,
                                                0x0F, 0xEE, 0xD8, 0xA0}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xEB, {0x02, 0x00, 0xE4, 0xE4, 0x88, 0x00, 0x40}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xEC, {0x3C, 0x00}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xED, {0xAB, 0x89, 0x76, 0x54, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x20,
                                                0x45, 0x67, 0x98, 0xBA}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x13}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xE5, {0xE4}),
    ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x00}),
    ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x11),
};
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIN_NUM_RGB_DISP            (-1)
#define EXAMPLE_LCD_PIN_NUM_RGB_VSYNC           (17)
#define EXAMPLE_LCD_PIN_NUM_RGB_HSYNC           (16)
#define EXAMPLE_LCD_PIN_NUM_RGB_DE              (18)
#define EXAMPLE_LCD_PIN_NUM_RGB_PCLK            (21)
                                                        // | RGB565 | RGB666 | RGB888 |
                                                        // |--------|--------|--------|
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA0           (4)     // |   B0   |  B0-1  |   B0-3 |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA1           (5)     // |   B1   |  B2    |   B4   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA2           (6)     // |   B2   |  B3    |   B5   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA3           (7)     // |   B3   |  B4    |   B6   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA4           (15)    // |   B4   |  B5    |   B7   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA5           (8)     // |   G0   |  G0    |   G0-2 |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA6           (20)    // |   G1   |  G1    |   G3   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA7           (3)     // |   G2   |  G2    |   G4   |
#if EXAMPLE_LCD_RGB_DATA_WIDTH > 8
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA8           (46)    // |   G3   |  G3    |   G5   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA9           (9)     // |   G4   |  G4    |   G6   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA10          (10)    // |   G5   |  G5    |   G7   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA11          (11)    // |   R0   |  R0-1  |   R0-3 |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA12          (12)    // |   R1   |  R2    |   R4   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA13          (13)    // |   R2   |  R3    |   R5   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA14          (14)    // |   R3   |  R4    |   R6   |
#define EXAMPLE_LCD_PIN_NUM_RGB_DATA15          (0)     // |   R4   |  R5    |   R7   |
#endif
#define EXAMPLE_LCD_PIN_NUM_SPI_CS              (39)
#define EXAMPLE_LCD_PIN_NUM_SPI_SCK             (48)
#define EXAMPLE_LCD_PIN_NUM_SPI_SDA             (47)
#define EXAMPLE_LCD_PIN_NUM_RST                 (-1)    // Set to -1 if not used
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT            (38)    // Set to -1 if not used
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL           (1)
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL          !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

/* Enable or disable printing RGB refresh rate */
#define EXAMPLE_ENABLE_PRINT_LCD_FPS            (1)

#define _EXAMPLE_LCD_CLASS(name, ...) ESP_PanelLcd_##name(__VA_ARGS__)
#define EXAMPLE_LCD_CLASS(name, ...)  _EXAMPLE_LCD_CLASS(name, ##__VA_ARGS__)

#if EXAMPLE_ENABLE_PRINT_LCD_FPS
#define EXAMPLE_LCD_FPS_COUNT_MAX               (100)

DRAM_ATTR int fps = 0;

IRAM_ATTR bool onVsyncEndCallback(void *user_data)
{
    DRAM_ATTR static int frame_count = 0;
    DRAM_ATTR static long frame_start_time = 0;

    if (frame_start_time == 0) {
        frame_start_time = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= EXAMPLE_LCD_FPS_COUNT_MAX) {
        fps = EXAMPLE_LCD_FPS_COUNT_MAX * 1000 / (millis() - frame_start_time);
        frame_count = 0;
        frame_start_time = millis();
    }

    return false;
}
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println("3-wire SPI + RGB LCD example start");

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Initialize backlight and turn it off");
    ESP_PanelBacklight *backlight = new ESP_PanelBacklight(
        EXAMPLE_LCD_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL, true
    );
    backlight->begin();
    backlight->off();
#endif

    Serial.println("Create 3-wire SPI + RGB LCD bus");
#if EXAMPLE_LCD_RGB_DATA_WIDTH == 8
    ESP_PanelBusRGB *lcd_bus = new ESP_PanelBusRGB(
        EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
        EXAMPLE_LCD_PIN_NUM_SPI_CS, EXAMPLE_LCD_PIN_NUM_SPI_SCK, EXAMPLE_LCD_PIN_NUM_SPI_SDA,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA0, EXAMPLE_LCD_PIN_NUM_RGB_DATA1, EXAMPLE_LCD_PIN_NUM_RGB_DATA2,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA3, EXAMPLE_LCD_PIN_NUM_RGB_DATA4, EXAMPLE_LCD_PIN_NUM_RGB_DATA5,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA6, EXAMPLE_LCD_PIN_NUM_RGB_DATA7, EXAMPLE_LCD_PIN_NUM_RGB_HSYNC,
        EXAMPLE_LCD_PIN_NUM_RGB_VSYNC, EXAMPLE_LCD_PIN_NUM_RGB_PCLK, EXAMPLE_LCD_PIN_NUM_RGB_DE,
        EXAMPLE_LCD_PIN_NUM_RGB_DISP
    );
#elif EXAMPLE_LCD_RGB_DATA_WIDTH == 16
    ESP_PanelBusRGB *lcd_bus = new ESP_PanelBusRGB(
        EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
        EXAMPLE_LCD_PIN_NUM_SPI_CS, EXAMPLE_LCD_PIN_NUM_SPI_SCK, EXAMPLE_LCD_PIN_NUM_SPI_SDA,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA0, EXAMPLE_LCD_PIN_NUM_RGB_DATA1, EXAMPLE_LCD_PIN_NUM_RGB_DATA2,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA3, EXAMPLE_LCD_PIN_NUM_RGB_DATA4, EXAMPLE_LCD_PIN_NUM_RGB_DATA5,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA6, EXAMPLE_LCD_PIN_NUM_RGB_DATA7, EXAMPLE_LCD_PIN_NUM_RGB_DATA8,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA9, EXAMPLE_LCD_PIN_NUM_RGB_DATA10, EXAMPLE_LCD_PIN_NUM_RGB_DATA11,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA12, EXAMPLE_LCD_PIN_NUM_RGB_DATA13, EXAMPLE_LCD_PIN_NUM_RGB_DATA14,
        EXAMPLE_LCD_PIN_NUM_RGB_DATA15, EXAMPLE_LCD_PIN_NUM_RGB_HSYNC, EXAMPLE_LCD_PIN_NUM_RGB_VSYNC,
        EXAMPLE_LCD_PIN_NUM_RGB_PCLK, EXAMPLE_LCD_PIN_NUM_RGB_DE, EXAMPLE_LCD_PIN_NUM_RGB_DISP
    );
#endif
    lcd_bus->configRgbTimingFreqHz(EXAMPLE_LCD_RGB_TIMING_FREQ_HZ);
    lcd_bus->configRgbTimingPorch(
        EXAMPLE_LCD_RGB_TIMING_HPW, EXAMPLE_LCD_RGB_TIMING_HBP, EXAMPLE_LCD_RGB_TIMING_HFP,
        EXAMPLE_LCD_RGB_TIMING_VPW, EXAMPLE_LCD_RGB_TIMING_VBP, EXAMPLE_LCD_RGB_TIMING_VFP
    );
    lcd_bus->configRgbBounceBufferSize(EXAMPLE_LCD_RGB_BOUNCE_BUFFER_SIZE); // Set bounce buffer to avoid screen drift
    lcd_bus->begin();

    Serial.println("Create LCD device");
    ESP_PanelLcd *lcd = new EXAMPLE_LCD_CLASS(EXAMPLE_LCD_NAME, lcd_bus, EXAMPLE_LCD_COLOR_BITS, EXAMPLE_LCD_PIN_NUM_RST);
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
    // Configure external initialization commands, should called before `init()`
    lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd)/sizeof(lcd_init_cmd[0]));
#endif
    // lcd->configEnableIO_Multiplex(true); // If the "3-wire SPI" interface are sharing pins of the "RGB" interface to
                                            // save GPIOs, please enable this function to release the bus object and pins
                                            // (except CS signal). And then, the "3-wire SPI" interface cannot be used to
                                            // transmit commands any more.
    // lcd->configMirrorByCommand(true);    // This function is conflict with `configAutoReleaseBus(true)`, please don't
                                            // enable them at the same time
    lcd->init();
    lcd->reset();
    lcd->begin();
    lcd->displayOn();
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    /* Attach a callback function which will be called when the Vsync signal is detected */
    lcd->attachRefreshFinishCallback(onVsyncEndCallback, nullptr);
#endif

    Serial.println("Draw color bar from top left to bottom right, the order is B - G - R");
    /* Users can refer to the implementation within `colorBardTest()` to draw patterns on the LCD */
    lcd->colorBarTest(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT);

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Turn on the backlight");
    backlight->on();
#endif

    Serial.println("3-wire SPI + RGB LCD example end");
}

void loop()
{
    delay(1000);
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    Serial.println("RGB refresh rate: " + String(fps));
#else
    Serial.println("IDLE loop");
#endif
}
