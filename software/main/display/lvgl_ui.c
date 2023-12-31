/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  lvgl_ui.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 10, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "lvgl_ui.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <math.h>
#include "lvgl.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef PI
#define PI  (3.14159f)
#endif

// LVGL image declare
LV_IMG_DECLARE(ui_img_background_png)
LV_IMG_DECLARE(ui_img_rhomboid_png)
LV_IMG_DECLARE(ui_img_ue_logo_png)
LV_IMG_DECLARE(ui_img_status_png)
//LV_IMG_DECLARE(esp_text)
LV_IMG_DECLARE(ui_img_useful_electronics_png)
typedef struct {
    lv_obj_t *scr;
    int count_val;
} my_timer_context_t;

static my_timer_context_t my_tim_ctx;
static lv_obj_t * btn;
static lv_obj_t *arc[3];
static lv_obj_t *img_logo;
static lv_obj_t *img_text = NULL;
static lv_color_t arc_color[] = {
//    LV_COLOR_MAKE(232, 87, 116),
	LV_COLOR_MAKE(22, 242, 255),
    LV_COLOR_MAKE(255, 255, 255),
    LV_COLOR_MAKE(90, 202, 228),
};
 lv_obj_t * ui_Screen1;
 lv_obj_t * ui_Image1;
 lv_obj_t * ui_Panel1;
 lv_obj_t * ui_Panel2;
 lv_obj_t * ui_Label1;
 lv_obj_t * ui_Label2;

lv_obj_t *display;
lv_obj_t *tv1;
lv_obj_t *tv2;

void tv2_screen_init(void);

static void anim_timer_cb(lv_timer_t *timer)
{
    my_timer_context_t *timer_ctx = (my_timer_context_t *) timer->user_data;
    int count = timer_ctx->count_val;
    lv_obj_t *scr = timer_ctx->scr;

    // Play arc animation
    if (count < 90) {
        lv_coord_t arc_start = count > 0 ? (1 - cosf(count / 180.0f * PI)) * 270 : 0;
        lv_coord_t arc_len = (sinf(count / 180.0f * PI) + 1) * 135;

        for (size_t i = 0; i < sizeof(arc) / sizeof(arc[0]); i++) {
            lv_arc_set_bg_angles(arc[i], arc_start, arc_len);
            lv_arc_set_rotation(arc[i], (count + 120 * (i + 1)) % 360);
        }
    }

    // Delete arcs when animation finished
    if (count == 90) {
        for (size_t i = 0; i < sizeof(arc) / sizeof(arc[0]); i++) {
            lv_obj_del(arc[i]);
        }

        // Create new image and make it transparent
        img_text = lv_img_create(scr);
        lv_img_set_src(img_text, &ui_img_useful_electronics_png);
        lv_obj_set_style_img_opa(img_text, 0, 0);
    }

    // Move images when arc animation finished
    if ((count >= 100) && (count <= 180)) {
        lv_coord_t offset = (sinf((count - 140) * 2.25f / 90.0f) + 1) * 20.0f;
        lv_obj_align(img_logo, LV_ALIGN_CENTER, 0, -offset);
        lv_obj_align(img_text, LV_ALIGN_CENTER, 0,  offset);
        lv_obj_set_style_img_opa(img_text, offset / 40.0f * 255, 0);
    }

    // Delete timer when all animation finished
    if ((count += 5) == 220) {
        lv_timer_del(timer);
        //switch screen
        lv_obj_set_tile_id(display, 0, 1, LV_ANIM_ON);

        // Enable button
//        lv_obj_clear_state(btn, LV_STATE_DISABLED);
    } else {
        timer_ctx->count_val = count;
    }
}

static void start_animation(lv_obj_t *scr)
{
    // Align image
    lv_obj_center(img_logo);

    // Create arcs
    for (size_t i = 0; i < sizeof(arc) / sizeof(arc[0]); i++) {
        arc[i] = lv_arc_create(scr);

        // Set arc caption
        lv_obj_set_size(arc[i], 220 - 30 * i, 220 - 30 * i);
        lv_arc_set_bg_angles(arc[i], 120 * i, 10 + 120 * i);
        lv_arc_set_value(arc[i], 0);

        // Set arc style
        lv_obj_remove_style(arc[i], NULL, LV_PART_KNOB);
        lv_obj_set_style_arc_width(arc[i], 10, 0);
        lv_obj_set_style_arc_color(arc[i], arc_color[i], 0);

        // Make arc center
        lv_obj_center(arc[i]);
    }

    if (img_text) {
        lv_obj_del(img_text);
        img_text = NULL;
    }

    // Create timer for animation
    my_tim_ctx.count_val = -90;
    my_tim_ctx.scr = scr;
    lv_timer_create(anim_timer_cb, 50, &my_tim_ctx);

    // Disable button
//    lv_obj_add_state(btn, LV_STATE_DISABLED);
}

static void btn_cb(lv_event_t * e)
{
    lv_obj_t * scr = lv_event_get_user_data(e);
    start_animation(scr);
}

void tv2_screen_init(void)
{

    lv_obj_clear_flag(tv2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Image1 = lv_img_create(tv2);
    lv_img_set_src(ui_Image1, &ui_img_background_png);
    lv_obj_set_width(ui_Image1, LV_SIZE_CONTENT);   /// 240
    lv_obj_set_height(ui_Image1, LV_SIZE_CONTENT);    /// 320
    lv_obj_set_x(ui_Image1, -2);
    lv_obj_set_y(ui_Image1, 0);
    lv_obj_set_align(ui_Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel1 = lv_obj_create(tv2);
    lv_obj_set_width(ui_Panel1, 70);
    lv_obj_set_height(ui_Panel1, 70);
    lv_obj_set_x(ui_Panel1, 0);
    lv_obj_set_y(ui_Panel1, 100);
    lv_obj_set_align(ui_Panel1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_Panel1, &ui_img_status_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_Panel1, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label2 = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "99");
    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0x181C62), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2 = lv_obj_create(tv2);
    lv_obj_set_width(ui_Panel2, 195);
    lv_obj_set_height(ui_Panel2, 187);
    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui_Panel2, &ui_img_rhomboid_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_Panel2, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label1 = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "cm");
    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0x0B5093), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void lvgl_distance_set(uint16_t distance)
{
	char temp_distance_string[10] = {0};

	sprintf(temp_distance_string, "%d", distance);

	lv_label_set_text(ui_Label1, temp_distance_string);
}

void lvgl_packet_count_set(uint32_t packet_count)
{
	char temp_distance_string[10] = {0};

	sprintf(temp_distance_string, "%d", (uint16_t)packet_count);

	lv_label_set_text(ui_Label2, temp_distance_string);
}

void lvgl_communication_status(bool communication_status)
{
	if(communication_status)
	{
		lv_obj_add_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);
	}
	else
	{
		lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_HIDDEN);
	}

}



void lvgl_demo_ui(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);

    lv_theme_t * theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);

	//Initialize 2 tiles that act as pages
    display = lv_tileview_create(scr);
	lv_obj_align(display, LV_ALIGN_TOP_RIGHT, 0, 0);
	tv1 = lv_tileview_add_tile(display, 0, 0, LV_DIR_HOR);
	tv2 = lv_tileview_add_tile(display, 0, 1, LV_DIR_HOR);

	tv2_screen_init();
    // Create image

	lv_obj_set_tile_id(display, 0, 0, LV_ANIM_ON);
    img_logo = lv_img_create(tv1);
    lv_img_set_src(img_logo, &ui_img_ue_logo_png);


    start_animation(tv1);
}


/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
