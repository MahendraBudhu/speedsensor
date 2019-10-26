#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://speed-sensor.firebaseio.com"                         
#define FIREBASE_AUTH "9Yy5G2WtGoWC7JaGc5zYtvk62uqNhaqNOPMK7RE7"                    
#define WIFI_SSID "Ganesh"                                          
#define WIFI_PASSWORD "Mahendra123" 

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;

int SensorA = 5; 
int SensorB = 4;  
int i=1;
int isObstacle = HIGH;   
unsigned long startTime;
unsigned long endTime;
unsigned long totalTime;
float hour;
float totalSpeed;
float distance=0.00005;
String path = "/Speeds/Speed(kmh) ";
void setup(){
  pinMode(SensorA, INPUT);
  pinMode(SensorB, INPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  }


void loop()
{
 isObstacle = digitalRead(SensorA);
  if (isObstacle == LOW){
    startTime = millis();;
  }
    isObstacle = digitalRead(SensorB);
  if (isObstacle == LOW){
    endTime = millis();
    delay(200);
   speedfunc(startTime, endTime);
  }
}
void speedfunc (long x, long y){
    totalTime = y - x;
    hour=0.000000277778*totalTime;
    totalSpeed=distance/hour;
    Serial.print("Speed: ");
    Serial.print(totalSpeed);
    Serial.println("km/h"); 
    Firebase.setDouble(firebaseData, path + i, totalSpeed);
    i++;
}
