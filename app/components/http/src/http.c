
#include <http.h>

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"


#include "esp_http_client.h"

#if CONFIG_SSL_USING_WOLFSSL
#include "lwip/apps/sntp.h"
#endif

#define MAX_HTTP_RECV_BUFFER 512


static const char *TAG = "[COMPONENTS HTTP]";

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // Write out data
                 printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

void http_test_task(void *pvParameters){

	
	esp_http_client_config_t config = {
		 .url = "http://httpbin.org/get",
		 .event_handler = _http_event_handler,
	 };
	 esp_http_client_handle_t client = esp_http_client_init(&config);
	
	 // GET
	 esp_err_t err = esp_http_client_perform(client);
	 if (err == ESP_OK) {
		 ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
				 esp_http_client_get_status_code(client),
				 esp_http_client_get_content_length(client));
	 } else {
		 ESP_LOGE(TAG, "HTTP GET request failed: %d", err);

	 }
	 vTaskDelete(NULL);

}

void xHttpGetResponse(){
	xTaskCreate(&http_test_task, "http_test_task", 2048, NULL, 10, NULL);


}

