/**
 * # LVGL Rotation Example
 *
 * The example demonstrates how to use LVGL(v8) to rotate the display.
 *
 * ## How to Use
 *
 * To use this example, please firstly install the following dependent libraries:
 *
 * - lvgl (>= v8.3.9, < v9)
 *
 * Follow the steps below to configure:
 *
 * 1. For **ESP32_Display_Panel**:
 *
 *     - Follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-drivers) to configure drivers if needed.
 *     - If using a supported development board, follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#using-supported-development-boards) to configure it.
 *     - If using a custom board, follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#using-custom-development-boards) to configure it.
 *
 * 2. For **lvgl**:
 *
 *     - Follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-lvgl) to add *lv_conf.h* file and change the configurations.
 *     - Modify the macros in the [lvgl_port_v8.h](./lvgl_port_v8.h) file to configure the LVGL porting parameters.
 *
 * 3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters. For supported
 *    boards, please refter to [Configuring Supported Development Boards](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-supported-development-boards)
 * 4. Verify and upload the example to your ESP board.
 *
 * ## Serial Output
 *
 * ```bash
 * ...
 * LVGL rotation example start
 * Initialize panel device
 * Initialize LVGL
 * Create UI
 * LVGL rotation example end
 * IDLE loop
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
#include <ESP_Panel_Library.h>
#include <lvgl.h>
#include "lvgl_port_v8.h"

#if LVGL_PORT_AVOID_TEAR
    #error "This example does not support the avoid tearing function. Please use `LVGL_PORT_ROTATION_DEGREE` for rotation"
#endif

LV_IMG_DECLARE(img_esp_logo);

static lv_obj_t *lbl_rotation = NULL;
static lv_disp_rot_t rotation = LV_DISP_ROT_NONE;

static void rotateDisplay(lv_disp_t *disp, lv_disp_rot_t rotation)
{
    lvgl_port_lock(-1);
    lv_disp_set_rotation(disp, rotation);
    lv_label_set_text_fmt(lbl_rotation, "Rotation %d°", (int)rotation * 90);
    lvgl_port_unlock();
}

static void onRightBtnClickCallback(lv_event_t *e)
{
    if (rotation == LV_DISP_ROT_270) {
        rotation = LV_DISP_ROT_NONE;
    } else {
        rotation = (lv_disp_rot_t)(((int)rotation) + 1);
    }
    /* Rotate display */
    rotateDisplay(lv_disp_get_default(), rotation);
}

static void onLeftBtnClickCallback(lv_event_t *e)
{
    if (rotation == LV_DISP_ROT_NONE) {
        rotation = LV_DISP_ROT_270;
    } else {
        rotation = (lv_disp_rot_t)(((int)rotation) - 1);
    }
    /* Rotate display */
    rotateDisplay(lv_disp_get_default(), rotation);
}

void setup()
{
    String title = "LVGL rotation example";

    Serial.begin(115200);
    Serial.println(title + " start");

    Serial.println("Initialize panel device");
#ifdef IM
    pinMode(IM1, OUTPUT);
    digitalWrite(IM1, HIGH);
  #ifdef BOARD_UEDX24320028E_WB_A_3_5_320_480
    pinMode(IM0, OUTPUT);
    digitalWrite(IM0, HIGH);
  #endif
  #ifndef BOARD_UEDX24320028E_WB_A_3_5_320_480
    pinMode(IM0, OUTPUT);
    digitalWrite(IM0, LOW);
  #endif
#endif
    ESP_Panel *panel = new ESP_Panel();
    panel->init();
#if LVGL_PORT_AVOID_TEAR
    // When avoid tearing function is enabled, configure the RGB bus according to the LVGL configuration
    ESP_PanelBus_RGB *rgb_bus = static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus());
    rgb_bus->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
    rgb_bus->configRgbBounceBufferSize(LVGL_PORT_RGB_BOUNCE_BUFFER_SIZE);
#endif
    panel->begin();

    Serial.println("Initialize LVGL");
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    Serial.println("Create UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    lv_obj_t *scr = lv_scr_act();
    lv_obj_t *lbl = NULL;

    // Create image
    lv_obj_t *img_logo = lv_img_create(scr);
    lv_img_set_src(img_logo, &img_esp_logo);
    lv_obj_align(img_logo, LV_ALIGN_TOP_MID, 0, 20);

    lbl_rotation = lv_label_create(scr);
    lv_label_set_text(lbl_rotation, "Rotation 0°");
    lv_obj_align(lbl_rotation, LV_ALIGN_CENTER, 0, 20);

    lv_obj_t *cont_row = lv_obj_create(scr);
    lv_obj_set_size(cont_row, panel->getLcdHeight() - 10, 50);
    lv_obj_align(cont_row, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_top(cont_row, 5, 0);
    lv_obj_set_style_pad_bottom(cont_row, 5, 0);
    lv_obj_set_style_pad_left(cont_row, 5, 0);
    lv_obj_set_style_pad_right(cont_row, 5, 0);
    lv_obj_set_flex_align(cont_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Button rotate left */
    lv_obj_t *btn_left = lv_btn_create(cont_row);
    lbl = lv_label_create(btn_left);
    lv_label_set_text_static(lbl, LV_SYMBOL_LEFT" Left");
    lv_obj_align(btn_left, LV_ALIGN_BOTTOM_LEFT, 30, -30);
    /* Button event */
    lv_obj_add_event_cb(btn_left, onLeftBtnClickCallback, LV_EVENT_CLICKED, NULL);

    lbl = lv_label_create(cont_row);
    lv_label_set_text_static(lbl, " rotate ");

    /* Button rotate right */
    lv_obj_t *btn_right = lv_btn_create(cont_row);
    lbl = lv_label_create(btn_right);
    lv_label_set_text_static(lbl, "Right "LV_SYMBOL_RIGHT);
    lv_obj_align(btn_right, LV_ALIGN_BOTTOM_LEFT, 30, -30);
    /* Button event */
    lv_obj_add_event_cb(btn_right, onRightBtnClickCallback, LV_EVENT_CLICKED, NULL);

    /* Release the mutex */
    lvgl_port_unlock();

    Serial.println(title + " end");
}

void loop()
{
    Serial.println("IDLE loop");
    delay(1000);
}
