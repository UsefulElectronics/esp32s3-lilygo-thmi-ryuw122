/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  main.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Nov 5, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "main.h"

/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
static const char *TAG = "main";
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/

#define SYS_TICK()				(xTaskGetTickCount() * portTICK_PERIOD_MS)
/* PRIVATE FUNCTIONS DECLARATION ---------------------------------------------*/
static void system_send_to_queue(void *tx_buffer, uint8_t command_length);

static void system_uwb_callback(void* rx_data, uint8_t packetId);

static void uart_reception_task(void *param);

static void anchor_periodic_send_task(void *param);

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void app_main(void)
{

	gpio_config_output(PIN_NUM_BK_LIGHT);
	gpio_config_output(PWR_ON_PIN);
	gpio_config_output(PWR_EN_PIN);

	uart_config();

	i80_controller_init((void*)gpio_set_level);

	ryuw122_init(system_send_to_queue, system_uwb_callback, RYUW122_ANCHOR);

    ESP_LOGI(TAG, "Display LVGL animation");

    xTaskCreatePinnedToCore(uart_event_task, "uart event", 10000, NULL, 4, NULL, 0);

    xTaskCreatePinnedToCore(uart_transmission_task, "USART TX handling task", 10000, NULL, 4, NULL, 0);

    xTaskCreatePinnedToCore(uart_reception_task, "USART RX handling task", 10000, NULL, 4, NULL, 0);

    xTaskCreatePinnedToCore(anchor_periodic_send_task, "Anchor periodic send task", 10000, NULL, 4, NULL, 0);

    while (1)
    {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
    }
}

static void system_send_to_queue(void *tx_buffer, uint8_t command_length)
{
	uartHandler_t temp_buffer = {0};

	memcpy(temp_buffer.uart_txBuffer, tx_buffer, command_length);

	ESP_LOGI(TAG, "command :%s, size: %d", temp_buffer.uart_txBuffer, command_length);

	temp_buffer.uart_txPacketSize = command_length;

   xQueueSendToBack(uartTx_queue, (void*) &temp_buffer, portMAX_DELAY);
}

static void system_uwb_callback(void* rx_data, uint8_t packetId)
{
	static uint32_t packet_count = 0;

	uint16_t temp_distance = 0;

	switch (packetId)
	{
		case RYUW122_ANCHOR_DISTANCE:
			memcpy(&temp_distance, rx_data, 2);

			lvgl_communication_status(false);

			lvgl_distance_set(temp_distance);

			packet_count++;

			lvgl_packet_count_set(packet_count);

			break;

		case RYUW122_ANCHOR_RX:

			break;

		case RYUW122_TAG_RX:

			break;

		default:
			break;
	}


}

static void uart_reception_task(void *param)
{
   uartHandler_t uartHandler = {0};

   uint8_t movementType = 0;

   int16_t detectedDistance = 0;


   for(;;)
   {
      //Waiting for UART packet to get received.
      if(xQueueReceive(uartRxStore_queue, (void * )&uartHandler, portMAX_DELAY))
      {


    	  ryuw122_packet_separator((char*) uartHandler.uart_rxBuffer, uartHandler.uart_rxPacketSize);

      }
   }
}

static void anchor_periodic_send_task(void *param)
{
	static uint32_t return_idle_timer = 0;

	return_idle_timer =  SYS_TICK();

	const char* test_string = "test";
	for(;;)
	{

		if(ryuw122_anchor_send(test_string, 4) == false)
		{

			if((SYS_TICK() - return_idle_timer) >= 1000)
			{
				return_idle_timer = SYS_TICK();

				ryuw122_return_idle();

				lvgl_communication_status(true);
			}
		}


		vTaskDelay(200/portTICK_PERIOD_MS);
	}
}



/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
