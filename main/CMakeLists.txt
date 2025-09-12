idf_component_register(SRCS "spiffs_example_main.c"
                    INCLUDE_DIRS "."
                    
                   REQUIRES spiffs 
                   REQUIRES esp_wifi
                   REQUIRES nvs_flash
                   REQUIRES esp_http_client
                   REQUIRES esp_timer
                   
                    )
