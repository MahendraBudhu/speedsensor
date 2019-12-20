#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://pingpongmachine-d18ca.firebaseio.com/"                         
#define FIREBASE_AUTH "MqwvpzIN6TCz2hSClMNG83UF33O9wtBjOKbt3tIa"                    
#define WIFI_SSID "Ganesh"                                          
#define WIFI_PASSWORD "Mahendra123" 

FirebaseData firebaseData;
FirebaseJson json;
String currentUserPath = "Current User";
void setup(){
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
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  pinMode(4, OUTPUT);
  }


void loop() {
  //Getting user path/id
  String currentUser;
  if (Firebase.getString(firebaseData, currentUserPath)) {
    if (firebaseData.dataType() == "string") {
      currentUser = firebaseData.stringData();
      Serial.print("Current User: ");
      Serial.println(currentUser);
    }
  }//End of getting user path

  
  //Changing ball speed
  String ballspeedPath = "Users/" + currentUser + "/Ball Configuration /Ball Speed";
  if (Firebase.getString(firebaseData, ballspeedPath)) {
    if (firebaseData.dataType() == "string") {
      String ballSpeed = firebaseData.stringData();
      Serial.print("Ball Speed: ");
      Serial.println(ballSpeed);
       if(ballSpeed=="Fast"){
         Serial.println("on");
         digitalWrite(4, HIGH);
      }
       if(ballSpeed=="Medium"){
         digitalWrite(4, LOW);
         Serial.println("off");
      }
       if(ballSpeed=="Slow"){
         digitalWrite(4, LOW);
         Serial.println("off");
      }  
    }
  }//End of ballSpeed
  delay(100); 
}
