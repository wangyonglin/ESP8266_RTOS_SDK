#pragma once



#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	OFF,ON,BLINK
}led_tab_t;
	
void led(led_tab_t ltt);


#ifdef __cplusplus
}
#endif


