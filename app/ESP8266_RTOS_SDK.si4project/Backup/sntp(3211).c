#include <http.h>
#include <system.h>
#include <wifi.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "lwip/apps/sntp.h"

static const char *TAG = "[COMPONENTS SNTP]";
static void set_server_name(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, "ntp.aliyun.com");
	sntp_setservername(1, "ntp.ntsc.ac.cn");
	sntp_setservername(2, "cn.ntp.org.cn");
    sntp_init();
}

static void obtain_time(EventGroupHandle_t wifi_event_group,int CONNECTED_BIT)
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,false, true, portMAX_DELAY);
    set_server_name();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }
}

static void sntp_example_task(void *arg)
{
	wifi_handler_t *handler=(wifi_handler_t*)arg;
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    time(&now);
    localtime_r(&now, &timeinfo);

    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time(handler->wifi_event_group,handler->CONNECTED_BIT);
    }

    // Set timezone to Eastern Standard Time and print local time
    // setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    // tzset();

    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();

    while (1) {
        // update 'now' variable with current time
        time(&now);
        localtime_r(&now, &timeinfo);

        if (timeinfo.tm_year < (2016 - 1900)) {
            ESP_LOGE(TAG, "The current date/time error");
        } else {
            strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
            ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
        }

        ESP_LOGI(TAG, "Free heap size: %d\n", esp_get_free_heap_size());
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void initialise_sntp(xHandler_t *handler){

	// SNTP service uses LwIP, please allocate large stack space.
	xTaskCreate(sntp_example_task, "sntp_example_task", 2048, handler, 10, NULL);

}

