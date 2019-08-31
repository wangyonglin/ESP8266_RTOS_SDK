#pragma once



#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
		DH=64,
		DM=1024,
		DL=2048,
	 
}led_length_t;
	
void driver_led_init();

void LED(int delay);




#ifdef __cplusplus
}
#endif


