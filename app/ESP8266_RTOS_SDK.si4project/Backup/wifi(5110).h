#pragma once

#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	EventGroupHandle_t wifi_event_group;
	int CONNECTED_BIT;
	int ESPTOUCH_DONE_BIT;
	void * pvParameters;
}xHandler_t;

typedef void (*xWifiResult)(xHandler_t *handler,int status,void*parameters);


typedef struct{
	xWifiResult result;
	xHandler_t *handler;	
	int		status;
	void	*parameters;
}xWifiResultCallback_t;





void initialise_wifi(xWifiResult result);

#ifdef __cplusplus
}
#endif

