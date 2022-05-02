
#include <Wire.h>
#include <Adafruit_Sensor.h>    // Adafruit  sensor library
#include <Adafruit_MPU6050.h>
#include "WiFi.h"
#include <Arduino.h>
#include <FirebaseESP32.h>
/////////
// FIREBASE NAME/SECRET Config 
#define PROJECT_IO "vibration-project-9f1df-default-rtdb.firebaseio.com/"
#define DATA_SECRET "IbHU7wfiFQ2CqXcvPv5gcvgGvO68HSAXZoo4F546"
///////
FirebaseData firebaseData;
FirebaseJson json;
Adafruit_MPU6050 mpu;
////////////
int  n = 0;
int  m = 0;
float X_acc[800],Y_acc[800],Z_acc[800];
float cali_x, cali_y, cali_z;
String jsonStr;
///////////    
const char* ssid = "iPhone ของ User";
const char* password =  "076442482";

////////////////////////////////////////////////////////////
WiFiClient espClient;
//////////////////////////////////////////////
void setup() {
////// connection wifi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting to Connect WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  delay(2000);
////// connection firebase
  Firebase.begin(PROJECT_IO, DATA_SECRET);
  Firebase.reconnectWiFi(true);
  delay(5000);
////// connection sensor and setting
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
 }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
   delay(100);
   
//// calibate sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  cali_x = a.acceleration.x;
  cali_y = a.acceleration.y;
  cali_z = a.acceleration.z;
  Serial.print("cali_x:");
  Serial.println(cali_x);
  Serial.print("cali_y:");
  Serial.println(cali_y);
  Serial.print("cali_z:");
  Serial.println(cali_z);
  Serial.println("Calibration is complete. ");
  delay(3000);
  Serial.println("Run Machine");
  delay(10000);
}
void loop() {
//////////////////////// x-axis
Serial.println("x-axis");
 n=0;
 while( n < 800){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
 float x_acc = a.acceleration.x-cali_x;
 float nearest = roundf(x_acc * 100) / 100;
 X_acc[n] = nearest;
 Serial.println(X_acc[n]);
 n += 1;
  }
/////////////////////// y-axis
Serial.println("y-axis");
 n=0;
 while( n < 800){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
 float y_acc = a.acceleration.y-cali_y;
 float nearest = roundf(y_acc * 100) / 100;
 Y_acc[n] = nearest;
 Serial.println(Y_acc[n]);
 n += 1;
  }
////////////////////// z-axis
Serial.println("z-axis");
 n=0;
 while( n < 800){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
 float z_acc = a.acceleration.z-cali_z;
 float nearest = roundf(z_acc * 100) / 100;
 Z_acc[n] = nearest;
 Serial.println(Z_acc[n]);
 n += 1;
  }
  
//////////////////////////// upload firebase-X
  m=0;  
  while ( m < 800 ){
    json.set("X/["+String(m)+"]", X_acc[m]);
    m += 1;
  }
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println("dataX to json completed");
    Firebase.setJSON(firebaseData, "/vibration/machine-ex-01/X-axis",json);
    delay(500);
    Serial.println("upload X-axis completed");
  json.remove("X");
  
 /////////////////////////////////// upload firebase-Y
  m=0;  
  while ( m < 800 ){
    json.set("Y/["+String(m)+"]", Y_acc[m]);
    m += 1;
  }
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println("dataY to json completed");
    Firebase.setJSON(firebaseData, "/vibration/machine-ex-01/Y-axis",json);
    delay(500);
    Serial.println("upload Y-axis completed");
    json.remove("Y");
    
//////////////////////////////////// upload firebase-Z
  m=0;  
  while ( m < 800 ){
     json.set("Z/["+String(m)+"]", Z_acc[m]);
    m += 1;
  }
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println("dataZ to json completed");
    Firebase.setJSON(firebaseData, "/vibration/machine-ex-01/Z-axis",json);
    delay(500);
    Serial.println("upload Z-axis completed");
    json.remove("Z");
/////////////////////////////////////////////////
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
 
}
