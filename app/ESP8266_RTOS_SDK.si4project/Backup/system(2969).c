#include <system.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "rom/ets_sys.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"


static const char *TAG = "[components system]";

system_handle_t system_init()
{
	system_handle_t sht;
	sht.event=xEventGroupCreate();
	ESP_LOGI(TAG, "xEventGroupCreate Init OK:\r\n");
	return sht;
}