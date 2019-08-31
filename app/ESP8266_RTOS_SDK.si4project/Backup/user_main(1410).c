#include <wifi.h>
#include <system.h>
#include <gpio.h>

#include <stdio.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "[USER]";
void  application(void * parameters){
	 // ESP_LOGE(TAG, "wifi_connect_events");
}


void app_main(void)
{

	//获取IDF版本
    printf("SDK version:%s\n", esp_get_idf_version());

    //获取芯片可用内存
    printf("esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
    //获取从未使用过的最小内存
    printf("esp_get_minimum_free_heap_size : %d  \n", esp_get_minimum_free_heap_size());
    //获取芯片的内存分布，返回值具体见结构体 flash_size_map
    printf("system_get_flash_size_map(): %d \n", system_get_flash_size_map());

	//Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) 
	{
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
	
	 //initialise_wifi(application,NULL);
	 initialise_gpio(OFF);
}
