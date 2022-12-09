
#include <esp_now.h>
#include <WiFi.h>
#include <WiFiScan.h>

float Distance1 = 0, Distance2 = 0, Distance3 = 0;// Distance from each access points
float ratio1 = 0, ratio2 = 0, ratio3 = 0;// Ratio for distance calculation

float ms = 100; // WiFi scan interval
float ch1 = 7; // Channel of access point 1
float ch2 = 7; // Channel of access point 2
float ch3 = 6; // Channel of access point 3

int Ar1 = -61; // A for access point 1
int Ar2 = -57; // A for access point 2
int Ar3 = -57; // A for access point 3

float N1 = 2.43;// N for access point 1
float N2 = 2.85;// N for access point 2
float N3 = 2.78;// N for access point 3




// SERVER ESP32 MAC ADDRESS
uint8_t broadcastAddress[] = {0x40, 0x91, 0x51, 0x88, 0xd3, 0x88};

// Create an object for sending distance from three access points
typedef struct struct_message {
    int id; // must be unique for each sender board
    float d1;
    float d2;
    float d3;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
         
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {

  myData.id = 1; // 1 for human and 2 for robot
  

  int n = WiFi.scanNetworks(false, false, false, ms, ch1, "SSID-AP1");
    for (int i = 0; i < n; ++i)
    {
       if(WiFi.SSID(i) == "SSID-AP1")
          {
            Serial.print(WiFi.RSSI(i));
            Serial.print(",");
            ratio1 = ((Ar1) - (WiFi.RSSI(i)))/(N1*10);
            Distance1 = pow(10 , ratio1); 
            Serial.print(Distance1);
            Serial.print(",");
            Serial.print('\n');
          }
    }
    int m = WiFi.scanNetworks(false, false, false, ms, ch2, "SSID-AP2");
    for (int j = 0; j < m; ++j)
    {
       if(WiFi.SSID(j) == "SSID-AP2")
          {
            Serial.print(WiFi.RSSI(j));
            Serial.print(",");
            ratio2 = ((Ar2) - (WiFi.RSSI(j)))/(N2*10);
            Distance2 = pow(10 , ratio2); 
            Serial.print(Distance2);
            Serial.print(",");
            Serial.print('\n');
          }
    }
    int l = WiFi.scanNetworks(false, false, false, ms, ch3, "SSID-AP3");
    for (int k = 0; k < l; ++k)
    {
       if(WiFi.SSID(k) == "SSID-AP3")
          {
            Serial.print(WiFi.RSSI(k));
            Serial.print(",");
            ratio3 = ((Ar3) - (WiFi.RSSI(k)))/(N3*10);
            Distance3 = pow(10 , ratio3); 
            Serial.print(Distance3);
            Serial.print(",");
            Serial.print('\n');
          }
    }
    myData.d1 = Distance1;
    myData.d2 = Distance2;
    myData.d3 = Distance3;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10);
}
