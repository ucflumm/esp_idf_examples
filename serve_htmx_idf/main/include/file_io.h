#ifndef FILE_IO_H
#define FILE_IO_H

#include "esp_err.h"
#include <stddef.h>

typedef struct {
    char *content;
    size_t size;
} file_content_t;

void init_spiffs(void);
file_content_t read_file_from_spiffs(const char* path);

#endif // FILE_IO_H