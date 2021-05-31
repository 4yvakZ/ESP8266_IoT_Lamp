// Import required libraries
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_NeoPixel.h>

#include "page.h"

#define LED_PIN 5
#define LED_COUNT 60
#define BRIGHTNESS 50

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

float temperature = 0;
int brightness = 0;
int R = 0;
int G = 0;
int B = 0;
bool autobrightness = LOW;
bool rainbow = LOW;
bool updateLED = LOW;

// Replace with your network credentials
const char* ssid = "ADL_Kinetic";
const char* password = "ADLaboratory";

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

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

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
      updateLED = HIGH;
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
  if(rainbow){
    rainbowLed();
    updateLED = HIGH;
  } else if (updateLED) {
    colorWipe(strip.Color(R, G, B));
  }
  if(autobrightness){
    strip.setBrightness(brightness);
  } else {
    strip.setBrightness(BRIGHTNESS);
  }
  yield();
  temperature = millis()%100;
  brightness = millis()/10%100;
}

void colorWipe(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                        //  Pause for a moment
  }
}

void rainbowLed() {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    yield();
  }
}
