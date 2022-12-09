
#include "WiFi.h"
#include "WiFiScan.h"

int ms = 100; // WiFi Scanning Interval
int channel = 7; // Channel of AP

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(10);  
}

void loop()
{
 
  //WiFiScan

    int n = WiFi.scanNetworks(false, false, false, ms, channel, "SSID-AP" ); // Change SSID of AP
    for (int i = 0; i < n; ++i)
    {
       if(WiFi.SSID(i) == "SSID-AP") // Change SSID of AP
          {
            Serial.println(WiFi.RSSI(i));
          }
    }
 }
