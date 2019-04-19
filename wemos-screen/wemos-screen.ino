#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// SSID and password of the networks to check
// Add as network as desired
String ssids[3] = {"yourSSID1", "yourSSID2", "yourSSID3"};
String passwords[3] = {"pwdSSID1", "pwdSSID2", "pwdSSID3"};

// Test server IP
const char *host = "xxx.xxx.xxx.xxx";
// Server Port
const int port = 8080;

#define OLED_RESET 0 // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

// Init objects
WiFiClient wifi;
HTTPClient http;

int getBarsSignal(long rssi)
{
    // 5. High quality: 90% ~= -55db
    // 4. Good quality: 75% ~= -65db
    // 3. Medium quality: 50% ~= -75db
    // 2. Low quality: 30% ~= -85db
    // 1. Unusable quality: 8% ~= -96db
    // 0. No signal
    int bars;

    if (rssi > -55)
    {
        bars = 4;
    }
    else if (rssi<-55 & rssi> - 65)
    {
        bars = 3;
    }
    else if (rssi<-65 & rssi> - 75)
    {
        bars = 2;
    }
    else if (rssi<-75 & rssi> - 85)
    {
        bars = 1;
    }
    else if (rssi<-85 & rssi> - 96)
    {
        bars = 0;
    }
    return bars;
}

void setup()
{
    pinMode(A0, INPUT); // Set A0 converter as input to measure the battery tension
    Serial.begin(115200);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Initialize with the I2C addr 0x3C (for the 64x48)
    display.display();
}

void loop()
{
    byte available_networks = WiFi.scanNetworks();
    if (available_networks > 0)
    {
        for (int network = 0; network < available_networks; network++)
        {
            String ssid = WiFi.SSID(network);
            long rssi = WiFi.RSSI(network);
            int bars = getBarsSignal(rssi);
            Serial.print("RSSI:");
            Serial.println(rssi);
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);

            // Display network name, signal Strength and battery level
            display.setCursor(0, 0);
            display.print("SSID: ");
            display.setCursor(0, 8);
            display.println(ssid);
            display.setCursor(0, 32);
            display.print("Channel: ");
            display.setCursor(48, 32);
            display.println(WiFi.channel(network));
            display.setCursor(0, 40);
            display.print("Signal: ");
            display.setCursor(44, 40);
            display.println(bars);

            // Display signal quality bars
            for (int b = 0; b <= bars; b++)
            {
                display.fillRect(70 + (b * 4), 33 - (b * 4), 3, b * 4, WHITE);
                // display.fillRect(10 + (b*5),48 - (b*5),3,b*5,WHITE);
            }

            display.display();
            delay(2000);
        }
    }
}
