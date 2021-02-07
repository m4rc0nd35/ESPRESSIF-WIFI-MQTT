#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <wifi_feature.h>
#include "esp_log.h"

void app_main(void)
{
    esp_log_level_set("LOG", ESP_LOG_NONE);
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    ESP_LOGI("TAG", "ESP_WIFI_MODE_STA");
    // wifi_set_credentials("brisa-1745905", "sg3j4fnq");
    wifi_init_sta();
}
