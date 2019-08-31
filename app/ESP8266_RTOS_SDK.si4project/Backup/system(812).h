#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "rom/ets_sys.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	EventGroupHandle_t wifi_event_group;
	int CONNECTED_BIT;
	int ESPTOUCH_DONE_BIT;
}xTaskHandler_t;

void xTaskHandlerBuilder(xTaskHandler_t * xHandler);


#ifdef __cplusplus
}
#endif

