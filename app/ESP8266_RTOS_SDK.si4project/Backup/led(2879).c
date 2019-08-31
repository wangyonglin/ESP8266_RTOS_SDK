#include <led.h>
#include <system.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "[COMPONENTS LED]";



#define GPIO_OUTPUT_IO_0    15
#define GPIO_OUTPUT_IO_1    16
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))


static xTaskHandle xHandle =NULL;


void led_task_example(void*arg){
	int xTicksToDelay=*(int*)arg;
	   while (1) {	   	
		   	gpio_set_level(GPIO_OUTPUT_IO_1, 0);
			vTaskDelay(xTicksToDelay / portTICK_RATE_MS);	
			gpio_set_level(GPIO_OUTPUT_IO_1, 1);
			vTaskDelay(xTicksToDelay / portTICK_RATE_MS);	
	   }

}
void LED(int xTicksToDelay){
		gpio_config_t io_conf;
	   //disable interrupt
	   io_conf.intr_type = GPIO_INTR_DISABLE;
	   //set as output mode
	   io_conf.mode = GPIO_MODE_OUTPUT;
	   //bit mask of the pins that you want to set,e.g.GPIO15/16
	   io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	   //disable pull-down mode
	   io_conf.pull_down_en = 0;
	   //disable pull-up mode
	   io_conf.pull_up_en = 0;
	   //configure GPIO with the given settings
	   gpio_config(&io_conf);
	if(xHandle!=NULL){
		 ESP_LOGI(TAG, "led_task_example vTaskDelete \n");
		vTaskDelete(&xHandle);
	}
		 ESP_LOGI(TAG, "led_task_example xTaskCreate \n");
		 xTaskCreate(led_task_example, "led_task_example", 1024, &xTicksToDelay, 10, &xHandle );
	
	

}



