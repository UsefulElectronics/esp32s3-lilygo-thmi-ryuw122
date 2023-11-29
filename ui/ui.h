// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.6
// Project name: T-HMI

#ifndef _T-HMI_UI_H
#define _T-HMI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Image2;
extern lv_obj_t * ui_Label2;
extern lv_obj_t * ui_Image3;
extern lv_obj_t * ui_Label1;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_background_png);    // assets\background.png
LV_IMG_DECLARE(ui_img_status_png);    // assets\status.png
LV_IMG_DECLARE(ui_img_rhomboid_png);    // assets\rhomboid.png

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
