/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  lvgl_ui.h
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 10, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/

#ifndef MAIN_DISPLAY_LVGL_UI_H_
#define MAIN_DISPLAY_LVGL_UI_H_


/* INCLUDES ------------------------------------------------------------------*/
#include "lvgl.h"
/* MACROS --------------------------------------------------------------------*/

/* ENUMORATIONS --------------------------------------------------------------*/

/* STRUCTURES & TYPEDEFS -----------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/

/* FUNCTIONS DECLARATION -----------------------------------------------------*/
void lvgl_demo_ui(lv_disp_t *disp);

void lvgl_distance_set(uint16_t distance);

void lvgl_packet_count_set(uint32_t packet_count);

void lvgl_communication_status(bool communication_status);


#endif /* MAIN_DISPLAY_LVGL_UI_H_ */

/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
