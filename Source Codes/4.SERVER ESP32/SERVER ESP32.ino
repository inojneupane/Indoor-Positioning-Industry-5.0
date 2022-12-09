
#include <esp_now.h>
#include <WiFi.h>

int grid = 3; // Grid size where AP1 at (0,0), AP2 at (0,grid) and AP3 at (grid,0); 

// Create an object with id and relative distances between three access points
typedef struct struct_message {
  int id;
  float d1;
  float d2;
  float d3;
}struct_message;

float d = 0;
float thres = 1; // Threshold distance of 1m
int flag =0; // Flag to indicate proximity beyond threshold for further processing

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the distance values from each board
struct_message board1;
struct_message board2;

// Create an array with all the structures
struct_message boardsStruct[2] = {board1, board2};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  memcpy(&myData, incomingData, sizeof(myData));
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].d1 = myData.d1;
  boardsStruct[myData.id-1].d2 = myData.d2;
  boardsStruct[myData.id-1].d3 = myData.d3; 
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  float E0d1 = boardsStruct[0].d1; // Distance of Human from AP1
  float E0d2 = boardsStruct[0].d2; // Distance of Human from AP2
  float E0d3 = boardsStruct[0].d3; // Distance of Human from AP3
  float E1d1 = boardsStruct[1].d1; // Distance of Robot from AP1
  float E1d2 = boardsStruct[1].d2; // Distance of Robot from AP2
  float E1d3 = boardsStruct[1].d3; // Distance of Robot from AP3

  float E0x = (((sq(grid))+((sq(E0d1))-(sq(E0d2)))) / (2*(grid))); // x - cordinate of Human using trilateration
  float E0y = (((sq(grid))+((sq(E0d1))-(sq(E0d3)))) / (2*(grid))); // y - cordinate of Human using trilateration

  float E1x = (((sq(grid))+((sq(E1d1))-(sq(E1d2)))) / (2*(grid))); // x - cordinate of Robot using trilateration
  float E1y = (((sq(grid))+((sq(E1d1))-(sq(E1d3)))) / (2*(grid))); // y - cordinate of Robot using trilateration

  
  Serial.print(E0d1);
  Serial.print(",");
  Serial.print(E0d2);
  Serial.print(",");
  Serial.print(E0d3);
  Serial.print(",");
  Serial.print(E0x);
  Serial.print(",");
  Serial.print(E0y);

  
  Serial.print(",");
  Serial.print(E1d1);
  Serial.print(",");
  Serial.print(E1d2);
  Serial.print(",");
  Serial.print(E1d3);
  Serial.print(",");
  Serial.print(E1x);
  Serial.print(",");
  Serial.print(E1y);
  Serial.print(",");

  d = sqrt((sq(E1x - E0x) + sq(E1y - E0y))); // Distance between Human and Robot

  Serial.print(d);
  Serial.print('\n');
  if (d < thres)
    flag = 1;
   else
    flag = 0;
 
  delay(150);  
}
