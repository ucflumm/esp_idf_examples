#include "file_io.h"
#include "esp_spiffs.h"
#include <stdio.h>
#include <stdlib.h>
#include <esp_log.h>
#include <fcntl.h>

void init_spiffs(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE("SPIFFS", "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE("SPIFFS", "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE("SPIFFS", "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE("SPIFFS", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI("SPIFFS", "Partition size: total: %d, used: %d", total, used);
    }
}

file_content_t read_file_from_spiffs(const char *path) {
    ESP_LOGI("SPIFFS", "Attempting to open file: %s", path);
    
    FILE *file = fopen(path, "r");
    file_content_t result = { .content = NULL, .size = 0 };

    if (!file) {
        ESP_LOGE("FILE", "Failed to open file for reading");
        return result;
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    result.size = ftell(file);
    rewind(file);

    // Allocate memory for the file content
    result.content = (char *)malloc(result.size + 1);
    if (!result.content) {
        ESP_LOGE("MALLOC", "Failed to allocate memory for file content");
        fclose(file);
        return result;
    }

    // Read the file into memory
    size_t bytes_read = fread(result.content, 1, result.size, file);
    if (bytes_read < result.size) {
        ESP_LOGE("FREAD", "Failed to read file");
        free(result.content);
        result.content = NULL;
        result.size = 0;
        fclose(file);
        return result;
    }

    // Null-terminate the content
    result.content[result.size] = '\0';
    fclose(file);
    ESP_LOGE("FILE", "File read successfully");
    return result;
}