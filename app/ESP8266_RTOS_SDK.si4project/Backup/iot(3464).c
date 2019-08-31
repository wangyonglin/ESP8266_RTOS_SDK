   
#include <iot.h>
#include <wifi.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "MQTTClient.h"

/* FreeRTOS event group to signal when we are connected & ready to make a request */
//static EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
//const int CONNECTED_BIT = BIT0;

#define MQTT_CLIENT_THREAD_NAME         "mqtt_client_thread"
#define MQTT_CLIENT_THREAD_STACK_WORDS  4096
#define MQTT_CLIENT_THREAD_PRIO         8

static const char *TAG = "[COMPONENTS IOT]";
MQTTClient client;

/*
static void messageArrived(MessageData *data)
{
    ESP_LOGI(TAG, "Message arrived[len:%u]: %.*s", \
           data->message->payloadlen, data->message->payloadlen, (char *)data->message->payload);
}
*/
static void mqtt_client_thread(void *pvParameters)
{
	xHandler_t *handler =(xHandler_t *)pvParameters;
    char *payload = NULL;

    Network network;
    int rc = 0;



    ESP_LOGI(TAG, "sub:%s qos:%u pub:%s qos:%u pubinterval:%u payloadsize:%u",
              CONFIG_MQTT_SUB_TOPIC,
             CONFIG_DEFAULT_MQTT_SUB_QOS, CONFIG_MQTT_PUB_TOPIC, CONFIG_DEFAULT_MQTT_PUB_QOS,
             CONFIG_MQTT_PUBLISH_INTERVAL, CONFIG_MQTT_PAYLOAD_BUFFER);

    ESP_LOGI(TAG, "ver:%u clientID:%s keepalive:%d username:%s passwd:%s session:%d level:%u",
             CONFIG_DEFAULT_MQTT_VERSION, CONFIG_MQTT_CLIENT_ID,
             CONFIG_MQTT_KEEP_ALIVE, CONFIG_MQTT_USERNAME, CONFIG_MQTT_PASSWORD,
             CONFIG_DEFAULT_MQTT_SESSION, CONFIG_DEFAULT_MQTT_SECURITY);

    ESP_LOGI(TAG, "broker:%s port:%u", CONFIG_MQTT_BROKER, CONFIG_MQTT_PORT);

    ESP_LOGI(TAG, "sendbuf:%u recvbuf:%u sendcycle:%u recvcycle:%u",
             CONFIG_MQTT_SEND_BUFFER, CONFIG_MQTT_RECV_BUFFER,
             CONFIG_MQTT_SEND_CYCLE, CONFIG_MQTT_RECV_CYCLE);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);

    if (MQTTClientInit(&client, &network, 0, NULL, 0, NULL, 0) == false) {
        ESP_LOGE(TAG, "mqtt init err");
        vTaskDelete(NULL);
    }

    payload = malloc(CONFIG_MQTT_PAYLOAD_BUFFER);

    if (!payload) {
        ESP_LOGE(TAG, "mqtt malloc err");
    } else {
        memset(payload, 0x0, CONFIG_MQTT_PAYLOAD_BUFFER);
    }

    for (;;) {
        ESP_LOGI(TAG, "wait wifi connect...");
        xEventGroupWaitBits(handler->wifi_event_group, handler->CONNECTED_BIT, false, true, portMAX_DELAY);

        if ((rc = NetworkConnect(&network, CONFIG_MQTT_BROKER, CONFIG_MQTT_PORT)) != 0) {
            ESP_LOGE(TAG, "Return code from network connect is %d", rc);
            continue;
        }

        connectData.MQTTVersion = CONFIG_DEFAULT_MQTT_VERSION;
        connectData.clientID.cstring = CONFIG_MQTT_CLIENT_ID;
        connectData.keepAliveInterval = CONFIG_MQTT_KEEP_ALIVE;
        connectData.username.cstring = CONFIG_MQTT_USERNAME;
        connectData.password.cstring = CONFIG_MQTT_PASSWORD;
        connectData.cleansession = CONFIG_DEFAULT_MQTT_SESSION;
        ESP_LOGI(TAG, "MQTT Connecting");
        if ((rc = MQTTConnect(&client, &connectData)) != 0) {
            ESP_LOGE(TAG, "Return code from MQTT connect is %d", rc);
            network.disconnect(&network);
            continue;
        }

        ESP_LOGI(TAG, "MQTT Connected");

#if defined(MQTT_TASK)

        if ((rc = MQTTStartTask(&client)) != pdPASS) {
            ESP_LOGE(TAG, "Return code from start tasks is %d", rc);
        } else {
            ESP_LOGI(TAG, "Use MQTTStartTask");
        }

#endif

        if ((rc = MQTTSubscribe(&client, CONFIG_MQTT_SUB_TOPIC, CONFIG_DEFAULT_MQTT_SUB_QOS, handler->pvParameters)) != 0) {
            ESP_LOGE(TAG, "Return code from MQTT subscribe is %d", rc);
            network.disconnect(&network);
            continue;
        }

        ESP_LOGI(TAG, "MQTT subscribe to topic %s OK", CONFIG_MQTT_SUB_TOPIC);

        for (;;) {
        
            vTaskDelay(CONFIG_MQTT_PUBLISH_INTERVAL / portTICK_RATE_MS);
        }

        network.disconnect(&network);
    }

    ESP_LOGW(TAG, "mqtt_client_thread going to be deleted");
    vTaskDelete(NULL);
    return;
}

void initialise_iot(xHandler_t *handler,void *pvParameters)
{
	handler->pvParameters=pvParameters;
    esp_err_t ret = xTaskCreate(&mqtt_client_thread,
                      MQTT_CLIENT_THREAD_NAME,
                      MQTT_CLIENT_THREAD_STACK_WORDS,
                      handler,
                      MQTT_CLIENT_THREAD_PRIO,
                      NULL);

    if (ret != pdPASS)  {
        ESP_LOGE(TAG, "mqtt create client thread %s failed", MQTT_CLIENT_THREAD_NAME);
    }
}
void Iot_MQTTPublish(const char *topic,char	*payload){
		int rc=0;
	  MQTTMessage message;
      		message.qos = CONFIG_DEFAULT_MQTT_PUB_QOS;
            message.retained = 0;
            message.payload = payload;    
            message.payloadlen = strlen(payload);

            if ((rc=MQTTPublish(&client, topic, &message)) != 0) {
                ESP_LOGE(TAG, "Return code from MQTT publish is %d", rc);
            } else {
                ESP_LOGI(TAG, "MQTT published topic %s, len:%u heap:%u", topic, message.payloadlen, esp_get_free_heap_size());
            }

           
}