
#include "WiFi.h"
#include "WiFiScan.h"

int ms = 100; // WiFi Scanning Interval
int channel = 6; // Channel of AP



int A = -57; // Value of measured RSSI at distance of 1m from A calculation
int d = 2; // Distance at which N is being measured for

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

    int n = WiFi.scanNetworks(false, false, false, ms, channel, "SSID-AP"); // Change SSID of AP
    for (int i = 0; i < n; ++i)
    {
       if(WiFi.SSID(i) == "SSID-AP") // Change SSID of AP
          {
            
            float N = ((A - (WiFi.RSSI(i))) / (10 * (log10(d))));
            Serial.println(N);
            
          }
    }
 }
