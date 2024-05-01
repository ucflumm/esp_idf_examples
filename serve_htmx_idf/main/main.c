#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_spiffs.h"
#include "include/app_network.h"
#include "http_handlers.h"
#include "file_io.h"

// Forward declaration for starting the web server
void start_webserver(void);
void list_spiffs_content(void);

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    connect_wifi();

    init_spiffs();

    list_spiffs_content();

    start_webserver();
}

void start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    
    if (httpd_start(&server, &config) == ESP_OK) {
        
        // Register dynamic content handlers
        httpd_uri_t get_time_uri = {
            .uri       = "/get-time",
            .method    = HTTP_GET,
            .handler   = get_time_handler,
            .user_ctx  = NULL
        };

        httpd_register_uri_handler(server, &get_time_uri);
        // Register static file handler
        httpd_uri_t uri_handler = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = http_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_handler);

        httpd_uri_t uri_handler_general = {
            .uri       = "/*",  // Catch all other URIs
            .method    = HTTP_GET,
            .handler   = http_get_handler_general,  // Adjust this handler as needed
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_handler_general);       
    }
}