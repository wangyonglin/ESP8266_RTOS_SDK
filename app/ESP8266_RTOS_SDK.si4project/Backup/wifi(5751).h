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





xHandler_t * initialise_wifi(xWifiResult result,void	*parameters);

#ifdef __cplusplus
}
#endif

