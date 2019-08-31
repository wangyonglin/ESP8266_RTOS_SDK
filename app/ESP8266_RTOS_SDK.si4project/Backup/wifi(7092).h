#pragma once

#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*xWifiResult)(int status,void*parameters);


typedef struct{
	xWifiResult result;
	int		status;
	void	*parameters;
}xTaskFramework_t;


typedef struct {
	EventGroupHandle_t wifi_event_group;
	int CONNECTED_BIT;
	int ESPTOUCH_DONE_BIT;
}wifi_handler_t;


wifi_handler_t* initialise_wifi(xWifiResult result,void	*parameters);

#ifdef __cplusplus
}
#endif

