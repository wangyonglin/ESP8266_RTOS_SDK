#pragma once

#include <wifi.h>

#include "MQTTClient.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {	
	xHandler_t *handler;
	void *pvParameters;
}xIotCallback_t;


	void initialise_iot(xHandler_t *handler,void * pvParameters);
	void Iot_MQTTPublish(const char *topic,char	*payload);

#ifdef __cplusplus
}
#endif


