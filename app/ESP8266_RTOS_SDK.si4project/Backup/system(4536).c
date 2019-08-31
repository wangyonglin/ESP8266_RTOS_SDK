#include <system.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "rom/ets_sys.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"


static const char *TAG = "[components system]";

void xTaskHandlerBuilder(xTaskHandler_t * xHandler){
	if(xHandler==NULL){
		xHandler =(xHandler*)malloc(sizeof(xHandler));
		memset(xHandler,0,sizeof(xHandler));
	}
	xHandler->wifi_event_group=xEventGroupCreate();
	xHandler->CONNECTED_BIT = BIT0;
	xHandler->ESPTOUCH_DONE_BIT = BIT1;
}


