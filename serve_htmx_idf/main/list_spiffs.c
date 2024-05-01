#include "esp_spiffs.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "esp_log.h"

void list_spiffs_content(void) {
    ESP_LOGI("SPIFFS", "Listing files in SPIFFS:");
    char path[32];
    snprintf(path, sizeof(path), "/spiffs/");

    DIR* dir = opendir(path);
    if (!dir) {
        ESP_LOGE("SPIFFS", "Failed to open directory");
        return;
    }

    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        ESP_LOGI("SPIFFS", "Found file: %s", ent->d_name);
    }
    closedir(dir);
}