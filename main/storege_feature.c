#include <storege_feature.h>

static const char *TAG = "STOREGE";

char* storege_get(const char* key)
{
    esp_err_t err = nvs_flash_init();
    nvs_handle_t storege_handle;
    err = nvs_open("storage", NVS_READONLY, &storege_handle);
    size_t _size;
    nvs_get_str(storege_handle, key, NULL, &_size);
    char *sdata = malloc(_size);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "[ERROR] (%s) opening NVS handle!\n", esp_err_to_name(err));
        return NULL;
    } else {
        err = nvs_get_str(storege_handle, key, sdata, &_size);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "[SUCCESS] Leitura");
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGI(TAG, "[CRITICAL] Valor não inicializado!");
                break;
            default :
                ESP_LOGI(TAG, "[ERROR] Lendo (%s)", esp_err_to_name(err));
        }
        nvs_close(storege_handle);
    }
    return sdata;
}

esp_err_t storege_set(const char* key, char* data)
{
    esp_err_t err = nvs_flash_init();
    nvs_handle_t storege_handle;
    err = nvs_open("storage", NVS_READWRITE, &storege_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_set_str(storege_handle, key, data);
        printf((err != ESP_OK) ? "Failed!\n" : "Writed done\n");
        ESP_LOGI(TAG, "Committing updates in NVS ... ");
        err = nvs_commit(storege_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Commit done\n");
        nvs_close(storege_handle);
    }
    return err;
}