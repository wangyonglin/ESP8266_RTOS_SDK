#include <uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h" 

static const char *TAG = "[COMPONENTS IOT]";

#define BUF_SIZE (1024)

static void echo_task(void *pdata)
{
   xUartCallback_t * callback=(xUartCallback_t*)pdata;

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        if(len>0){
			callback->result(data);
		}
    }
}

void xUartWrite(char *data,int len)	{
	uart_write_bytes(UART_NUM_1, (const char *) data, len);
}

void initialise_uart(void * result){

    xUartCallback_t * callback=(xUartCallback_t*)malloc(sizeof(xUartCallback_t));
	callback->result=result;
	uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL);

	xTaskCreate(echo_task, "uart_echo_task", 1024, callback, 10, NULL);
}

