#include <wifi_feature.h>

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "WIFI-STA";

void on_wifi_connect(
    void *esp_netif, 
    esp_event_base_t event_base,
    int32_t event_id, 
    void *event_data
){
    gpio_set_level(WIFI_STATUS_GPIO, 1);
}
void on_wifi_disconnect(
    void *arg, 
    esp_event_base_t event_base,
    int32_t event_id, 
    void *event_data
){
    ESP_LOGI(TAG, "Disconnected, trying to reconnect...");
    gpio_set_level(WIFI_STATUS_GPIO, 0);
    esp_err_t err = esp_wifi_connect();
    if (err == ESP_ERR_WIFI_NOT_STARTED) {
        return;
    }
    ESP_ERROR_CHECK(err);
}
void wifi_init_sta(void)
{
    gpio_pad_select_gpio(WIFI_STATUS_GPIO);
    gpio_set_direction(WIFI_STATUS_GPIO, GPIO_MODE_OUTPUT);

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &on_wifi_connect, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &on_wifi_disconnect, NULL));

    wifi_config_t wifi_config = {0};
    strcpy((char*)wifi_config.sta.ssid, storege_get("ssid"));
    strcpy((char*)wifi_config.sta.password, storege_get("wifi_password"));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
    ESP_LOGI(TAG, "Finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            CONNECTED_BITS,
            true,
            true,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        gpio_set_level(WIFI_STATUS_GPIO, 1);
        ESP_LOGI(TAG, "Connected to AP!");
    }else if (bits & WIFI_FAIL_BIT) {
        gpio_set_level(WIFI_STATUS_GPIO, 0);
        ESP_LOGI(TAG, "Failed to connect!");
    }else
        ESP_LOGE(TAG, "UNEXPECTED EVENT");

    vEventGroupDelete(s_wifi_event_group);
}
void wifi_set_credentials(char *ssid, char *password)
{
    storege_set("ssid", ssid);
    storege_set("wifi_password", password);
    ESP_LOGI(TAG, "New Credentials SSID:%s PASSWORD:%s",ssid, password);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    esp_restart();
}