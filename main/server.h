#include <stdio.h>
#include <stdlib.h>
#include "esp_http_server.h"    
#include "server_callbacks.h"
#include "cJSON.h"




static void init_server()     // can be used to store paths
{
 
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();   //working configuration of our server

  config.uri_match_fn = httpd_uri_match_wildcard;   //enables the use of wildcards in routes, (/*)

  ESP_ERROR_CHECK(httpd_start(&server, &config));   //start the server

// websocket code
  httpd_uri_t web_socket_url = {
      .uri = "/ws",         
      .method = HTTP_GET,
      .handler = on_WEB_SOCKET_url,
      .is_websocket = true              //for websockets only
    };
  httpd_register_uri_handler(server, &web_socket_url);
//end websocket code

   httpd_uri_t setBrightness_url = {   
    .uri ="/api/setBrightness",                  
    .method = HTTP_GET,
    .handler = on_setBrightness_url                                
  };httpd_register_uri_handler(server,&setBrightness_url);  

    httpd_uri_t hello_world_url = {   
    .uri ="/api/hello-world",                  
    .method = HTTP_GET,
    .handler = on_hello_world_url                                
  };httpd_register_uri_handler(server,&hello_world_url);  

    httpd_uri_t ledOn_url = {   
    .uri ="/api/LedOn",                  
    .method = HTTP_GET,
    .handler = on_LedOn_url                                
  };httpd_register_uri_handler(server,&ledOn_url); 

    httpd_uri_t ledOff_url = {   
    .uri ="/api/LedOff",                  
    .method = HTTP_GET,
    .handler = on_LedOff_url                                
  };httpd_register_uri_handler(server,&ledOff_url); 




  httpd_uri_t default_url = {   //url = browser address
    .uri ="/*",                  //accept everything (/*)
    .method = HTTP_GET,
    .handler = on_default_url   
                                //with /* every url call this func, unless further in sequence
  };
  httpd_register_uri_handler(server,&default_url);    
}