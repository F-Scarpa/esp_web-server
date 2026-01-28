#include <stdio.h>
#include <stdlib.h>
#include "wifi_connect.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_http_server.h"    
#include "mdns.h"
//#include "toggle_led.h"
#include "cJSON.h"
//#include "pushBtn.h"
#include "secrets.h"
#include "pinInit/pinInit.h"

#include "server.h"
#include "esp_vfs.h"
#include "esp_vfs_fat.h"


static const char *BASE_PATH = "/store";   





//callback functions of urls 
/*
static esp_err_t on_toggle_led_url(httpd_req_t *req)
{
  char buffer[100];                                                   //space for payload as string
                                                                      // (in this case is small) {is_on:true}

  memset(&buffer,0, sizeof(buffer));                                  //clean buffer
  httpd_req_recv(req,buffer,req->content_len);                        //fill the buffer

  cJSON *payload = cJSON_Parse(buffer);                               //create a json object from buffer 
                                                                      //(string to JSON)
  cJSON *is_on_json = cJSON_GetObjectItem(payload,"is_on");           //extract json key value 

  bool is_on = cJSON_IsTrue(is_on_json);    //check if value is true
  cJSON_Delete(payload);                    //free memory
  toggle_led(is_on);                        //call function url

  httpd_resp_set_status(req,"204 NO CONTENT");      //set http status
  httpd_resp_send(req,NULL,0);                      //send http status
  return ESP_OK;
}






*/





//server
/*

static void init_server()     // can be used to store paths
{
 
  
//endpoints
  
  httpd_uri_t toggle_led_url = {
      .uri = "/api/toggle-led",         //dont use underscores in uri
      .method = HTTP_POST,
      .handler = on_toggle_led_url    //function called while uri is visited
    };
  httpd_register_uri_handler(server, &toggle_led_url);    //register the url in the server

  

*/

//end server


//mdns service for custom name
void start_mdns_service()
{
  mdns_init();
  mdns_hostname_set("tests");    //create our name for server, set the host  type hostname.local
  mdns_instance_name_set("LEARN esp32 thing");
}
void mount_fs()     //setup for fat memory
{

   esp_vfs_fat_mount_config_t esp_vfs_fat_mount_config = {
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    esp_vfs_fat_spiflash_mount_ro(BASE_PATH, "storage", &esp_vfs_fat_mount_config);   //base_path is /store
                                                                                      //2. partition name in 
                                                                                      //partition table
}



void app_main(void)
{
  pinInit();
  ESP_ERROR_CHECK(nvs_flash_init());
  //init_led();
  //init_btn();

  wifi_connect_init();
  ESP_ERROR_CHECK(wifi_connect_sta(WIFI_SSID, WIFI_PASSWORD, 10000));

  start_mdns_service();
  mount_fs();   
  init_server();

  

}









