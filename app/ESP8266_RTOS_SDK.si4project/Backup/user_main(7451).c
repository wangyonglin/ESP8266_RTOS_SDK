#include <wifi.h>
#include <system.h>
#include <http.h>
#include <aliyuniot.h>
#include <stdio.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "[USER]";

xHandler_t xHandler;

void  WifiResult(xHandler_t *handler,int status,void*parameters){

		ESP_LOGE(TAG, "wifi_connect_ok %d",status);
		switch (status)
			{
				case 200:
					initialise_wifi(handler);
					break;
				
				
			}
		
}

esp_err_t Initialize_nvs(){
	//Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) 
	{
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
	return ret;
}

void app_main(void)
{

	//获取IDF版本
    ESP_LOGI(TAG,"SDK version:%s\n", esp_get_idf_version());
    //获取芯片可用内存
    ESP_LOGI(TAG,"esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
    //获取从未使用过的最小内存
    ESP_LOGI(TAG,"esp_get_minimum_free_heap_size : %d  \n", esp_get_minimum_free_heap_size());
    //获取芯片的内存分布，返回值具体见结构体 flash_size_map
    ESP_LOGI(TAG,"system_get_flash_size_map(): %d \n", system_get_flash_size_map());	
    ESP_ERROR_CHECK(Initialize_nvs());	
	initialise_wifi(WifiResult);
	
}
