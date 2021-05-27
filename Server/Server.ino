// Import required libraries
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include "page.h"

float temperature = 0;
int brightness = 0;
int R = 0;
int G = 0;
int B = 0;
bool autobrightness = LOW;
bool rainbow = LOW;

// Replace with your network credentials
const char* ssid = "HOME";
const char* password = "new_kandarat6";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(temperature);
  } else if(var == "BRIGHTNESS"){
    return String(brightness);
  } else if(var == "R"){
    return String(R);
  } else if(var == "G"){
    return String(G);
  } else if(var == "B"){
    return String(B);
  } else if(var == "AUTOBRIGHTNESS"){
    if (autobrightness){
      return "checked";
    } else {
      return String();
    }
  } else if(var == "RAINBOW"){
    if (rainbow){
      return "checked";
    } else {
      return String();
    }
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temperature).c_str());
  });
  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(brightness).c_str());
  });
  server.on("/RangeR", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(R).c_str());
  });
  server.on("/RangeG", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(G).c_str());
  });
  server.on("/RangeB", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(B).c_str());
  });
  server.on("/Rainbow", HTTP_GET, [](AsyncWebServerRequest *request){
    if(rainbow){
      request->send_P(200, "text/plain", String("checked").c_str());
    } else {
      request->send_P(200, "text/plain", String("unchecked").c_str());
    }
  });
  server.on("/Autobrightness", HTTP_GET, [](AsyncWebServerRequest *request){
    if(autobrightness){
      request->send_P(200, "text/plain", String("checked").c_str());
    } else {
      request->send_P(200, "text/plain", String("unchecked").c_str());
    }
  });


  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String id;
    String value;
    if (request->hasParam("slider") & request->hasParam("value")) {
      id = request->getParam("slider")->value();
      value = request->getParam("value")->value();
      Serial.println(id+" "+value);
      if(id=="RangeR"){
        R = value.toInt();
      } else if (id=="RangeG"){
        G = value.toInt();
      } else if (id=="RangeB"){
        B = value.toInt();
      }
      Serial.printf("%d %d %d\n", R, G, B);
    } else if (request->hasParam("toggle") & request->hasParam("state")) {
      id = request->getParam("toggle")->value();
      value = request->getParam("state")->value();
      Serial.println(id+" "+value);
      if(id=="Rainbow"){
        rainbow = value.toInt();
      } else if (id=="Autobrightness"){
        autobrightness = value.toInt();
      }
      Serial.printf("%d %d\n", rainbow, autobrightness);
    }
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}

void loop() {
  temperature = millis()%100;
  brightness = millis()/10%100;
}
