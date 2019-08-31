#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "rom/ets_sys.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef void (*funcation_t)(void *val);

typedef struct{
	funcation_t ft;
	int Parameter;
	void * val;
}callback_t;

#ifdef __cplusplus
}
#endif

