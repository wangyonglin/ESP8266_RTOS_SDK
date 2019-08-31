#include <wifi.h>
#include <system.h>
#include <led.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"
#include "smartconfig_ack.h"

static const char *TAG = "[COMPONENTS WIFI]";

void smartconfig_example_task(void * parm);
xHandler_t xHandler;


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    /* For accessing reason codes in case of disconnection */
    system_event_info_t *info = &event->event_info;
	xWifiResultCallback_t * callback=(xWifiResultCallback_t *)ctx;
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:		
		esp_wifi_connect();
		callback->result(&xHandler,WIFI_EVENT_STA_START,NULL);
		LED(100);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(xHandler.wifi_event_group, xHandler.CONNECTED_BIT);
		callback->result(&xHandler,WIFI_EVENT_STA_GOT_IP,NULL);
		LED(2000);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        ESP_LOGE(TAG, "Disconnect reason : %d", info->disconnected.reason);
        if (info->disconnected.reason == WIFI_REASON_BASIC_RATE_NOT_SUPPORT) {
            /*Switch to 802.11 bgn mode */
            esp_wifi_set_protocol(ESP_IF_WIFI_STA, WIFI_PROTOCAL_11B | WIFI_PROTOCAL_11G | WIFI_PROTOCAL_11N);
        }
		callback->result(&xHandler,WIFI_EVENT_STA_DISCONNECTED,NULL);
		LED(100);
      //  esp_wifi_connect();
		xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
        xEventGroupClearBits(xHandler.wifi_event_group, xHandler.CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

void initialise_wifi(xWifiResult result)

{
	//初始化LED灯
	driver_led_init();
	
	xWifiResultCallback_t * callback =(xWifiResultCallback_t*)malloc(sizeof(xWifiResultCallback_t));
	memset(callback,0,sizeof(xWifiResultCallback_t));	
	callback->result=result;
	
    tcpip_adapter_init();
	
	xHandler.wifi_event_group=xEventGroupCreate();
	xHandler.CONNECTED_BIT= BIT0;
	xHandler.ESPTOUCH_DONE_BIT= BIT1;
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler,callback) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
	
}

static void sc_callback(smartconfig_status_t status, void *pdata)
{
    switch (status) {
        case SC_STATUS_WAIT:
            ESP_LOGI(TAG, "SC_STATUS_WAIT");
            break;
        case SC_STATUS_FIND_CHANNEL:
            ESP_LOGI(TAG, "SC_STATUS_FINDING_CHANNEL");
            break;
        case SC_STATUS_GETTING_SSID_PSWD:
            ESP_LOGI(TAG, "SC_STATUS_GETTING_SSID_PSWD");
            break;
        case SC_STATUS_LINK:
            ESP_LOGI(TAG, "SC_STATUS_LINK");
            wifi_config_t *wifi_config = pdata;
            ESP_LOGI(TAG, "SSID:%s", wifi_config->sta.ssid);
            ESP_LOGI(TAG, "PASSWORD:%s", wifi_config->sta.password);
            ESP_ERROR_CHECK( esp_wifi_disconnect() );
            ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, wifi_config) );
            ESP_ERROR_CHECK( esp_wifi_connect() );
            break;
        case SC_STATUS_LINK_OVER:
            ESP_LOGI(TAG, "SC_STATUS_LINK_OVER");
            if (pdata != NULL) {
                sc_callback_data_t *sc_callback_data = (sc_callback_data_t *)pdata;
                switch (sc_callback_data->type) {
                    case SC_ACK_TYPE_ESPTOUCH:
                        ESP_LOGI(TAG, "Phone ip: %d.%d.%d.%d", sc_callback_data->ip[0], sc_callback_data->ip[1], sc_callback_data->ip[2], sc_callback_data->ip[3]);
                        ESP_LOGI(TAG, "TYPE: ESPTOUCH");
                        break;
                    case SC_ACK_TYPE_AIRKISS:
                        ESP_LOGI(TAG, "TYPE: AIRKISS");
                        break;
                    default:
                        ESP_LOGE(TAG, "TYPE: ERROR");
                        break;
                }
            }
            xEventGroupSetBits(xHandler.wifi_event_group, xHandler.ESPTOUCH_DONE_BIT);
            break;
        default:
            break;
    }
}

void smartconfig_example_task(void * parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH_AIRKISS) );
    ESP_ERROR_CHECK( esp_smartconfig_start(sc_callback) );
    while (1) {
        uxBits = xEventGroupWaitBits(xHandler.wifi_event_group, xHandler.CONNECTED_BIT | xHandler.ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY); 
        if(uxBits & xHandler.CONNECTED_BIT) {
            ESP_LOGI(TAG, "WiFi Connected to ap");
        }
        if(uxBits & xHandler.ESPTOUCH_DONE_BIT) {
            ESP_LOGI(TAG, "smartconfig over");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}

