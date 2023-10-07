// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.2
// LVGL version: 8.3.6
// Project name: ArduinoDemo

#include "ui.h"
#include "ui_helpers.h"


///////////////////// VARIABLES ////////////////////


// SCREEN: ui_ScreenClock
void ui_ScreenClock_screen_init(void);
lv_obj_t *ui_ScreenClock;
lv_obj_t *ui_PanelClock;
lv_obj_t *ui_LabelTime;
void ui_event_ImageSet( lv_event_t * e);
lv_obj_t *ui_ImageSet;
lv_obj_t *ui_Labelcalendar;
lv_obj_t *ui_Labelweek;
lv_obj_t *ui_Labelweather;
lv_obj_t *ui_Labeltemperature;
lv_obj_t *ui_ImageCloudy;
lv_obj_t *ui_ImageRain;
lv_obj_t *ui_ImageSunny;
lv_obj_t *ui_ImageSnow;
lv_obj_t *ui_ImageWifi;
lv_obj_t *ui_ImageThunderstor;
lv_obj_t *ui_ImageAtmosphere;
lv_obj_t *ui_ImageDrizzle;


// SCREEN: ui_ScreenSet
void ui_ScreenSet_screen_init(void);
lv_obj_t *ui_ScreenSet;
void ui_event_ButtonSetWifi( lv_event_t * e);
lv_obj_t *ui_ButtonSetWifi;
lv_obj_t *ui_LabelSetWifi;
void ui_event_ButtonAlarm( lv_event_t * e);
lv_obj_t *ui_ButtonAlarm;
lv_obj_t *ui_LabelAlarm;
void ui_event_ButtonRetClock( lv_event_t * e);
lv_obj_t *ui_ButtonRetClock;
lv_obj_t *ui_ImageRetClock;
void ui_event_ButtonFactory( lv_event_t * e);
lv_obj_t *ui_ButtonFactory;
lv_obj_t *ui_LabelFactory;


// SCREEN: ui_ScreenWifiList
void ui_ScreenWifiList_screen_init(void);
lv_obj_t *ui_ScreenWifiList;
void ui_event_ButtonRetSet( lv_event_t * e);
lv_obj_t *ui_ButtonRetSet;
lv_obj_t *ui_ImageRetSet;
lv_obj_t *ui_LabelWifiList;
lv_obj_t *ui_SpinnerLoadWifi;


// SCREEN: ui_ScreenPassord
void ui_ScreenPassord_screen_init(void);
lv_obj_t *ui_ScreenPassord;
lv_obj_t *ui_TextPassword;
void ui_event_KeyboardPassword( lv_event_t * e);
lv_obj_t *ui_KeyboardPassword;
void ui_event_ButtonRetWifi( lv_event_t * e);
lv_obj_t *ui_ButtonRetWifi;
lv_obj_t *ui_ImageRetWifi;
lv_obj_t *ui_SpinnerLoadPassword;


// SCREEN: ui_ScreenAla
void ui_ScreenAla_screen_init(void);
lv_obj_t *ui_ScreenAla;
lv_obj_t *ui_Rollerhour;
lv_obj_t *ui_Rollerminute;
lv_obj_t *ui_Rollersecond;
lv_obj_t *ui_Labelcolon1;
lv_obj_t *ui_Labelcolon2;
void ui_event_ButtonConfirm( lv_event_t * e);
lv_obj_t *ui_ButtonConfirm;
lv_obj_t *ui_LabelConfirm;
void ui_event_ButtonRetSet2( lv_event_t * e);
lv_obj_t *ui_ButtonRetSet2;
lv_obj_t *ui_ImageRetSet2;
lv_obj_t *ui_LabelHour;
lv_obj_t *ui_LabelHour1;
lv_obj_t *ui_LabelHour2;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_ImageSet( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenSet, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSet_screen_init);
}
}
void ui_event_ButtonSetWifi( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenWifiList, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenWifiList_screen_init);
}
if ( event_code == LV_EVENT_CLICKED) {
      ButtonSetWifi_cb( e );
}
}
void ui_event_ButtonAlarm( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenAla, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenAla_screen_init);
}
if ( event_code == LV_EVENT_CLICKED) {
      ButtonAlarm_cb( e );
}
}
void ui_event_ButtonRetClock( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenClock, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenClock_screen_init);
}
}
void ui_event_ButtonFactory( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenClock, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenClock_screen_init);
}
if ( event_code == LV_EVENT_CLICKED) {
      Factory_cb( e );
}
}
void ui_event_ButtonRetSet( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenSet, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSet_screen_init);
}
if ( event_code == LV_EVENT_CLICKED) {
      ButtonRetSet_cb( e );
}
}
void ui_event_KeyboardPassword( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_keyboard_set_target(ui_KeyboardPassword,  ui_TextPassword);
}
}
void ui_event_ButtonRetWifi( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenWifiList, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSet_screen_init);
}
if ( event_code == LV_EVENT_CLICKED) {
      ButtonRetWifi_cb( e );
}
}
void ui_event_ButtonConfirm( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      Ala_confirm_cb( e );
}
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenClock, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenClock_screen_init);
}
}
void ui_event_ButtonRetSet2( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_ScreenSet, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenSet_screen_init);
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_ScreenClock_screen_init();
ui_ScreenSet_screen_init();
ui_ScreenWifiList_screen_init();
ui_ScreenPassord_screen_init();
ui_ScreenAla_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_ScreenClock);
}
