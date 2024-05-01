idf_component_register(SRCS "list_spiffs.c" "main.c" "app_network.c" "file_io.c" "http_handlers.c" "list_spiffs.c"
                    INCLUDE_DIRS "include")
spiffs_create_partition_image(storage data FLASH_IN_PROJECT)