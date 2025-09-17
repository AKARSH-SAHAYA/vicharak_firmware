/* SPIFFS filesystem example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_spiffs.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_wifi_types_generic.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "esp_timer.h"


static const char *TAG = "WIFI_HTTP";

static FILE* file =NULL;

static int total_bytes=0;

esp_err_t http_event_handler(esp_http_client_event_t *eve)
{ if(eve->event_id ==HTTP_EVENT_ON_DATA)
   {  int64_t start = esp_timer_get_time();
	   size_t writtern =fwrite(eve->data,1,eve->data_len,file);
	   fflush(file);
	   total_bytes+= writtern;
	   
	   int64_t end= esp_timer_get_time();
	   double time_taken = (end-start)/1000000.0;
	   double speed =(total_bytes/1024.0) /time_taken;
	   ESP_LOGI(TAG , "TOTAL WRITE SPEED IN SPIFFS %.2f kBps",speed);
	   
	  
	   
   }
  else if(eve->event_id==HTTP_EVENT_ON_FINISH)
   {  if(file){
		   fclose(file);
		   file=NULL;
	   }
	   
   }
  return ESP_OK;
}

void api_hit()
{   file=fopen("/spiffs/response.txt","w");
	esp_http_client_config_t api={
		.url="http://httpbin.org/get",
		.auth_type=HTTP_AUTH_TYPE_NONE,
		.buffer_size=2048,
		.event_handler=http_event_handler,
	};
	esp_http_client_handle_t http = esp_http_client_init(&api);
	ESP_ERROR_CHECK(esp_http_client_perform(http));
	esp_http_client_cleanup(http);
}





void wifi_event_handler(void* args, esp_event_base_t event_base,int32_t event_id , void* event_data)
{
	 if(event_base==WIFI_EVENT && event_id== WIFI_EVENT_STA_START)
	  {
		  esp_wifi_connect();
	  }
	 else if(event_base==WIFI_EVENT && event_id== WIFI_EVENT_STA_DISCONNECTED)
	 {
		 esp_wifi_connect();
	 }
	 else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
	 {
		 api_hit();
	 }
	
}
void wifi_start()
{ESP_ERROR_CHECK( nvs_flash_init());
  esp_netif_init();
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();
   
  wifi_init_config_t config= WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&config);
 esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL, NULL);
esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL, NULL);
  esp_wifi_set_mode(WIFI_MODE_STA);
  wifi_config_t wifi_station ={
	  .sta={
		  .ssid="Test",
		  .password="123456789",
		  .sae_pwe_h2e=WPA3_SAE_PWE_BOTH}
  };
  
  esp_wifi_set_config(WIFI_IF_STA,&wifi_station);
  esp_wifi_start();
  
  	
}

void spiffs_start()
{
	 esp_vfs_spiffs_conf_t  conf={
		 .base_path="/spiffs",
		 .max_files=5,
		 .format_if_mount_failed=false,
		 .partition_label=NULL
	 };
	ESP_ERROR_CHECK( esp_vfs_spiffs_register(&conf));
	
}



void app_main(void)
{
    spiffs_start();
    
    wifi_start();
   
}
