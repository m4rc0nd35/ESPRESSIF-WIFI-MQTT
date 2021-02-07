#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

char* storege_get(const char* key);
esp_err_t storege_set(const char* key, char* data);