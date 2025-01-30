#include "WiFi.h"
#include "DFRobotDFPlayerMini.h"
#include <Adafruit_NeoPixel.h>

#define PIN 4
#define NUMPIXELS 16
#define DELAYVAL 100

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#ifdef _AVR_
#include <avr/power.h>
#endif

DFRobotDFPlayerMini DFP;
const char* ssid = "ESP32";
const char* password = "espemoto123espemoto";
WiFiServer server(80);
String header;

int selectedRed = 0;
int selectedGreen = 0;
int selectedBlue = 0;

unsigned long previousPixelTime = 0;
int currentPixel = 0;

unsigned long rainbowStart = 0;
int rainbowStep = 0;

unsigned long blinkPreviousTime = 0;
bool blinkState = false;
unsigned long pulsePreviousTime = 0;
float pulseBrightness = 0.0;
bool pulseIncreasing = true;

void movingEffect() {
  unsigned long currentTime = millis();

  if (currentTime - previousPixelTime >= DELAYVAL) {
    previousPixelTime = currentTime;

    pixels.clear();

    pixels.setPixelColor(currentPixel, pixels.Color(selectedRed, selectedGreen, selectedBlue));
    pixels.show();

    currentPixel++;
    if (currentPixel >= NUMPIXELS) {
      currentPixel = 0;
    }
  }
}

void rainbowEffect() {
  unsigned long currentTime = millis();

  if (currentTime - rainbowStart >= 100) {
    rainbowStart = currentTime;

    float factor1, factor2;
    uint16_t ind;

    for (uint16_t j = 0; j < NUMPIXELS; j++) {
      ind = rainbowStep + j * 1;
      switch ((int)((ind % 60) / 20)) {
        case 0:
          factor1 = 1.0 - ((float)(ind % 60 - 0 * 20) / 20);
          factor2 = (float)((int)(ind - 0) % 60) / 20;
          pixels.setPixelColor(j, pixels.Color(255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2));
          break;
        case 1:
          factor1 = 1.0 - ((float)(ind % 60 - 1 * 20) / 20);
          factor2 = (float)((int)(ind - 20) % 60) / 20;
          pixels.setPixelColor(j, pixels.Color(0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2));
          break;
        case 2:
          factor1 = 1.0 - ((float)(ind % 60 - 2 * 20) / 20);
          factor2 = (float)((int)(ind - 40) % 60) / 20;
          pixels.setPixelColor(j, pixels.Color(0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2));
          break;
      }
    }

    if (rainbowStep >= 60) {
      rainbowStep = 0;
    } else {
      rainbowStep++;
    }

    pixels.show();
  }
}

void pickedColorEffect() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(selectedRed, selectedGreen, selectedBlue));
  }
  pixels.show();
}

// Blink Effect
void blinkEffect() {
  unsigned long currentTime = millis();
  
  if (currentTime - blinkPreviousTime >= 500) {
    blinkPreviousTime = currentTime;
    
    if (blinkState) {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
    } else {
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(selectedRed, selectedGreen, selectedBlue));
      }
    }
    blinkState = !blinkState;
    pixels.show();
  }
}

void pulseEffect() {
  unsigned long currentTime = millis();
  
  if (currentTime - pulsePreviousTime >= 10) {
    pulsePreviousTime = currentTime;

    if (pulseIncreasing) {
      pulseBrightness += 0.01;
      if (pulseBrightness >= 0.98) {
        pulseIncreasing = false;
      }
    } else {
      pulseBrightness -= 0.01;
      if (pulseBrightness <= 0.02) {
        pulseIncreasing = true;
      }
    }

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(
        selectedRed * pulseBrightness,
        selectedGreen * pulseBrightness,
        selectedBlue * pulseBrightness
      ));
    }
    pixels.show();
  }
}

void site(WiFiClient& client) {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<link rel=\"stylesheet\" href=\"https://unpkg.com/picnic\">";
  html += "<style>body {background-color: #121212; color: white;} ";
  html += "h1 {color: #00ff00;} ";
  html += "select, input[type='color'] {background-color: #333333; color: white; border: none; border-radius: 5px;} ";
  html += "label {font-size: 18px; color: #ff0000;} ";
  html += "</style></head>";

  html += "<body><h1>ESP32 Motor Theme Web Server</h1>";

  html += "<label for=\"music\">Pusc muzyke:</label>";
  html += "<select id=\"music\" onchange=\"location = this.value;\">";
  html += "<option value=\"/MENU\">MENU</option>";
  html += "<option value=\"/music/1\">Muzyka 1</option>";
  html += "<option value=\"/music/2\">Muzyka 2</option>";
  html += "<option value=\"/music/3\">Muzyka 3</option>";
  html += "<option value=\"/music/4\">Muzyka 4</option>";
  html += "<option value=\"/music/5\">Muzyka 5</option>";
  html += "<option value=\"/STOP\">STOP</option>";
  html += "</select>";

  html += "<br><br>";
  html += "<label for=\"action\">Choose mode:</label>";
  html += "<select id=\"action\" onchange=\"location = this.value;\">";
  html += "<option value=\"/MENU\">MENU</option>";
  html += "<option value=\"/blink\">BLINK</option>";
  html += "<option value=\"/pulse\">PULSE</option>";
  html += "<option value=\"/rainbow\">RAINBOW</option>";
  html += "<option value=\"/still\">STILL</option>";
  html += "<option value=\"/moving\">MOVING</option>";
  html += "<option value=\"/stop\">STOP</option>";
  html += "</select>";

  html += "<br><br>";
  html += "<label for=\"color\">Color picker:</label>";
  html += "<input type=\"color\" id=\"color\" onchange=\"location = '/color/' + this.value.substring(1);\">";

  html += "</body></html>";
  client.println(html);
}

bool movingEffectBool = false;
bool rainbowEffectBool = false;
bool pickedColorEffectBool = false;
bool blinkEffectBool = false;
bool pulseEffectBool = false;

void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);

  DFP.begin(Serial2);
  DFP.setTimeOut(500);
  DFP.volume(30);
  DFP.EQ(0);

  WiFi.softAP(ssid, password);
  server.begin();

  pixels.begin();
  pixels.show();
}

void clearEffects()
{
  movingEffectBool = false;
  rainbowEffectBool = false;
  pickedColorEffectBool = false;
  blinkEffectBool = false;
  pulseEffectBool = false;
  pixels.clear();
  pixels.show();
}

void updateEffects() {
  if (movingEffectBool) {
    movingEffect();
  } else if (rainbowEffectBool) {
    rainbowEffect();
  } else if (pickedColorEffectBool) {
    pickedColorEffect();
  } else if (blinkEffectBool) {
    blinkEffect();
  } else if (pulseEffectBool) {
    pulseEffect();
  }
}

void loop() {
  // Handle Wi-Fi client
  WiFiClient client = server.available();
  if (client) {
    unsigned long currentTime = millis();
    unsigned long previousTime = currentTime;
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= 2000) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            if (header.indexOf("GET /music/1") >= 0) {
              DFP.stop();
              DFP.play(1);
            } else if (header.indexOf("GET /music/2") >= 0) {
              DFP.stop();
              DFP.play(2);
            } else if (header.indexOf("GET /music/3") >= 0) {
              DFP.stop();
              DFP.play(3);
            } else if (header.indexOf("GET /music/4") >= 0) {
              DFP.stop();
              DFP.play(4);
            } else if (header.indexOf("GET /music/5") >= 0) {
              DFP.stop();
              DFP.play(5);
            } else if (header.indexOf("GET /STOP") >= 0) {
              DFP.stop();
            } else if (header.indexOf("GET /moving") >= 0) {
              clearEffects();
              movingEffectBool = true;
            } 
            else if (header.indexOf("GET /stop") >= 0) {
              clearEffects();
            }
            else if (header.indexOf("GET /blink") >= 0) {
              clearEffects();
              blinkEffectBool = true;
            } 
            else if (header.indexOf("GET /rainbow") >= 0) {
              clearEffects();
              rainbowEffectBool = true;
            }
            else if (header.indexOf("GET /still") >= 0) {
              clearEffects();
              pickedColorEffectBool = true;
            }
            else if (header.indexOf("GET /pulse") >= 0) {
              clearEffects();
              pulseEffectBool = true;
            }
            else if (header.indexOf("GET /color/") >= 0) {
              int colorIndex = header.indexOf("GET /color/") + 11;
              String colorHex = header.substring(colorIndex, colorIndex + 6);

              long color = strtol(colorHex.c_str(), NULL, 16);
              selectedRed = (color >> 16) & 0xFF;
              selectedGreen = (color >> 8) & 0xFF;
              selectedBlue = color & 0xFF;

              pixels.clear();
              pixels.show();
            }
            site(client);
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
  updateEffects();
}