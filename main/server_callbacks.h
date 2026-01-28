#include "esp_log.h"

static const char *TAG = "SERVER";
static httpd_handle_t server = NULL;   //server handle


//websocket helper func and init//
#define WS_MAX_SIZE 1024
static int client_session_id;
esp_err_t send_ws_message(char* message)
{
  if(!client_session_id)
  {
    ESP_LOGE(TAG, "no client id");
    return -1;
  }
  httpd_ws_frame_t ws_message = {
    .final = true,
    .fragmented = false,
    .len = strlen(message),
    .payload = (uint8_t *)message,
    .type = HTTPD_WS_TYPE_TEXT

  };
  httpd_ws_send_frame_async(server, client_session_id, &ws_message);
  return 1;
}

static esp_err_t on_WEB_SOCKET_url(httpd_req_t *req)
{
  client_session_id = httpd_req_to_sockfd(req);
  if(req->method == HTTP_GET) return ESP_OK;
  
  httpd_ws_frame_t ws_pkt;
  memset(&ws_pkt,0,sizeof(httpd_ws_frame_t));
  ws_pkt.type = HTTPD_WS_TYPE_TEXT;
  ws_pkt.payload = malloc(WS_MAX_SIZE);         //create our own heap memory
  httpd_ws_recv_frame(req,&ws_pkt,WS_MAX_SIZE);   //copy memory 
  printf("ws payload: %.*s\n", ws_pkt.len, ws_pkt.payload);     //1. length to print 2. start index pointer 
  free(ws_pkt.payload);

  char* response = "{\"status\":\"connected OK\"}";
  httpd_ws_frame_t ws_response = {
    .final = true,
    .fragmented = false,
    .type = HTTPD_WS_TYPE_TEXT,
    .payload = (uint8_t *)response,
    .len = strlen(response), 
  };
  return httpd_ws_send_frame(req, &ws_response);
  
}
//end websocket//

static esp_err_t on_hello_world_url(httpd_req_t *req)
{
  httpd_resp_sendstr(req,"Hello world");
  printf("Hello noob");
  return ESP_OK;
}

static esp_err_t on_LedOn_url(httpd_req_t *req)
{
    httpd_resp_sendstr(req,"led on called");
    gpio_set_level(myPin,true);
    return ESP_OK;
}

static esp_err_t on_LedOff_url(httpd_req_t *req)
{
    httpd_resp_sendstr(req,"led off called");
    gpio_set_level(myPin,false);
    return ESP_OK;
}

static esp_err_t on_setBrightness_url(httpd_req_t *req)
{
    char buffer[32];
    int value  = 0;
        // read query ?value=XXX
    if (httpd_req_get_url_query_str(req, buffer, sizeof(buffer)) == ESP_OK) {
        char param[16];
        if (httpd_query_key_value(buffer, "value", param, sizeof(param)) == ESP_OK) {
            value = atoi(param);           // turn string in integer
        }
    }

    if (value < 0) value = 0;
    if (value > 100) value = 100;

    uint32_t brightness = value * 10.23;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, brightness);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    httpd_resp_sendstr(req,"setBrightness called");
    return ESP_OK;
}

static esp_err_t on_default_url(httpd_req_t *req)       //url for default page
{                                                     //http home url page
    ESP_LOGI(TAG,"URL: %s",req->uri);

    char path[600];                           //path buffer
    sprintf(path, "/store%s", req->uri);      //write a string inside a variable (inside path buffer)
                                              //uri for default is /*

    //******************    setup mimes    *******************************


    char* ext = strrchr(req -> uri, '.');
    if(ext)
    {
      if(strcmp(ext,".css") == 0) httpd_resp_set_type(req,"text/css");
      if(strcmp(ext,".js") == 0) httpd_resp_set_type(req,"text/javascript");
      if(strcmp(ext,".png") == 0) httpd_resp_set_type(req,"image/png");
      if(strcmp(ext,".jpg") == 0) httpd_resp_set_type(req,"image/jpg");
      if(strcmp(ext,".svg") == 0) httpd_resp_set_type(req,"image/svg+xml");
    };

    //******************************************************************** 
    FILE *file = fopen(path, "r"); 
    //if path does not exist                   
    if(file == NULL)
    {
      file = fopen("/store/index.html", "r");    //open index page
      if(file == NULL)
      {
        httpd_resp_send_404(req);   //send error 404
      }
      ESP_LOGI(TAG, "STORE/INDEX.HTML NOT FOUND");
    }
    char buffer[1024];
    int bytes_read = 0;
    while((bytes_read = fread(buffer, sizeof(char), sizeof(buffer),file)) > 0)   //read the file, end file
                                                                                 //when byres_read is 0
    {
      httpd_resp_send_chunk(req, buffer, bytes_read); //send response while buffer is filled with file data
    }
    fclose(file);   //close file
    httpd_resp_send_chunk(req, NULL, 0); //empty http data chunk

    
    return ESP_OK;
}