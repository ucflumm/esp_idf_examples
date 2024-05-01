#include "http_handlers.h"
#include "file_io.h"
#include <string.h>
#include "esp_log.h"

#define FILE_PATH_SIZE 600  // Increased size to accommodate maximum possible URI length

esp_err_t http_get_handler(httpd_req_t *req) {
    char filepath[FILE_PATH_SIZE];
    if (strcmp(req->uri, "/") == 0) {
        snprintf(filepath, FILE_PATH_SIZE, "/spiffs/index.html");
    } else {
        snprintf(filepath, FILE_PATH_SIZE, "/spiffs%s", req->uri);
    }

    ESP_LOGI("HTTP", "Serving file: %s", filepath);
    file_content_t file_data = read_file_from_spiffs(filepath);
    if (file_data.content == NULL) {
        ESP_LOGE("HTTP", "File not found: %s", filepath);
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    const char *ext = strrchr(filepath, '.');
    if (ext) {
        if (strcmp(ext, ".html") == 0) {
            httpd_resp_set_type(req, "text/html");
        } else if (strcmp(ext, ".css") == 0) {
            httpd_resp_set_type(req, "text/css");
        } else if (strcmp(ext, ".js") == 0) {
            httpd_resp_set_type(req, "application/javascript");
        } else {
            httpd_resp_set_type(req, "text/plain");
        }
    }

    httpd_resp_send(req, file_data.content, file_data.size);
    free(file_data.content);
    return ESP_OK;
}

esp_err_t get_time_handler(httpd_req_t *req) {
    char* resp_str = malloc(150); // Allocate more space to accommodate HTML tags
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    
    strftime(resp_str, 256, "<div id='time-display'>%H:%M:%S</div>", timeinfo);  // Format time with HTML

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, resp_str, strlen(resp_str));
    free(resp_str);
    return ESP_OK;
}

esp_err_t http_get_handler_general(httpd_req_t *req) {
    char filepath[FILE_PATH_SIZE];
    snprintf(filepath, FILE_PATH_SIZE, "/spiffs%s", req->uri);

    // Log the attempted file path to help debugging
    ESP_LOGI("HTTP", "Attempting to serve file: %s", filepath);

    file_content_t file_data = read_file_from_spiffs(filepath);
    if (file_data.content == NULL) {
        ESP_LOGE("HTTP", "File not found: %s", filepath);
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    // Determine and set the MIME type
    const char *ext = strrchr(filepath, '.');
    if (ext) {
        if (strcmp(ext, ".html") == 0) {
            httpd_resp_set_type(req, "text/html");
        } else if (strcmp(ext, ".css") == 0) {
            httpd_resp_set_type(req, "text/css");
        } else if (strcmp(ext, ".js") == 0) {
            httpd_resp_set_type(req, "application/javascript");
        } else {
            httpd_resp_set_type(req, "text/plain");
        }
    }

    httpd_resp_send(req, file_data.content, file_data.size);
    free(file_data.content);
    return ESP_OK;
}

httpd_uri_t get_time_uri = {
    .uri       = "/get-time",
    .method    = HTTP_GET,
    .handler   = get_time_handler,
    .user_ctx  = NULL
};
// Path: main/include/http_handlers.h