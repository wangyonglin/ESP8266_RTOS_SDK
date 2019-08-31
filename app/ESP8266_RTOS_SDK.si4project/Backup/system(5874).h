#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "rom/ets_sys.h"


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		EventGroupHandle_t	event;
	
	}system_handle_t;

	system_handle_t system_init();

#ifdef __cplusplus
}
#endif

