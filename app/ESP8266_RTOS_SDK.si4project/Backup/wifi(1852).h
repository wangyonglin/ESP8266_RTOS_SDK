#pragma once

#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif


	


typedef struct{
	TaskFunction_t pvTaskCode;
	unsigned short usStackDepth;
}xTaskFramework_t;


typedef struct {
	EventGroupHandle_t wifi_event_group;
	int CONNECTED_BIT;
	int ESPTOUCH_DONE_BIT;
}wifi_handler_t;


wifi_handler_t* initialise_wifi(TaskFunction_t pvTaskCode,unsigned short usStackDepth);

#ifdef __cplusplus
}
#endif

