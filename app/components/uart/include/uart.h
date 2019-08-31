#pragma once

#include <wifi.h>

#include "MQTTClient.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*xUartResult)(uint8_t *data);

typedef struct {
	xUartResult result;
	void *pvParameters;
}xUartCallback_t;


	void initialise_uart(void * result );
	void xUartWrite(char *data,int len);
#ifdef __cplusplus
}
#endif


