#ifndef HTTP_HANDLERS_H
#define HTTP_HANDLERS_H

#include "esp_http_server.h"
#include <time.h>

esp_err_t http_get_handler(httpd_req_t *req);
esp_err_t get_time_handler(httpd_req_t *req);
esp_err_t http_get_handler_general(httpd_req_t *req);

#endif // HTTP_HANDLERS_H