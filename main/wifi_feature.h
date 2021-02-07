#ifndef _WIFI_FEATURE_H_
#define _WIFI_FEATURE_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <storege_feature.h>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define WIFI_STATUS_GPIO 2
#define GOT_IPV4_BIT BIT(0)
#define GOT_IPV6_BIT BIT(1)
#define CONNECTED_BITS (GOT_IPV4_BIT)

/* Functions */
void on_wifi_connect(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void on_wifi_disconnect(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void wifi_init_sta(void);
void wifi_set_credentials(char *ssid, char *password);

#endif