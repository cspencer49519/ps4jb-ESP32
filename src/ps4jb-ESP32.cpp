#include <Arduino.h>

#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "dns_hijack_srv.h"


const char * AP_SSID = "ESP32 6.72 jailbreak server";

const IPAddress AP_IP( 192, 168, 5, 1 );

const uint8_t ONBOARD_LED = 2;

//DNS settings
const byte DNS_PORT = 53;
String host = "ps4-esp32-exploit";

AsyncWebServer server(80);

void WiFiEvent( WiFiEvent_t event )
{
  switch ( event )
  {
    case SYSTEM_EVENT_AP_START:
      WiFi.softAPConfig ( AP_IP, AP_IP, IPAddress( 255, 255, 255, 0 ) );
      Serial.print( "1. Connect your PS4 to '" );
      Serial.print( AP_SSID );
      Serial.println( "' WiFi access point." );
      Serial.println();
      Serial.print( "2. Browse to 'http://");
      Serial.print( WiFi.softAPIP() );
      Serial.println( "/' to jailbreak your PS4 6.72." );
      pinMode( ONBOARD_LED, OUTPUT );
      digitalWrite( ONBOARD_LED, HIGH );
      break;
    default:
      break;
  }
}

void setup()
{
  Serial.begin( 115200 );
  Serial.println();
  btStop();

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // setup access point

  WiFi.disconnect();   //added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); //added to start with the wifi off, avoid crashing
  WiFi.mode( WIFI_AP );
  WiFi.softAP( AP_SSID );
  WiFi.onEvent( WiFiEvent );

  ip4_addr_t resolve_ip;
  inet_pton(AF_INET, "192.168.5.1", &resolve_ip);
  
  if(dns_hijack_srv_start(resolve_ip) == ESP_OK) {
      ESP_LOGI(TAG, "DNS hijack server started");
  } else {
      ESP_LOGE(TAG, "DNS hijack server has not started");
  }

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    request->redirect("/index.html");
  });

  server.on("manuals.playstation.net", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
    response->addHeader("location", String("http://") + WiFi.softAPIP());
    request->send(response);
  });

  server.begin();
}

void loop() {}