#pragma once

#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif


	
typedef void (*wifi_result_t)(void * parameters);

typedef struct{
	wifi_result_t result;
	void * parameters;
}wifi_callback_t;


typedef struct {
	EventGroupHandle_t wifi_event_group;
	int CONNECTED_BIT;
	int ESPTOUCH_DONE_BIT;
}wifi_handler_t;

wifi_handler_t* initialise_wifi(wifi_result_t result,void * parameters);

#ifdef __cplusplus
}
#endif

