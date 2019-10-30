#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://pingpongmachine-d18ca.firebaseio.com/"                         
#define FIREBASE_AUTH "MqwvpzIN6TCz2hSClMNG83UF33O9wtBjOKbt3tIa"                    
#define WIFI_SSID "Ganesh"                                          
#define WIFI_PASSWORD "Mahendra123" 

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;

#include <Wire.h>

//Click here to get the library: http://librarymanager/All#SparkFun_VCNL4040
#include "SparkFun_VCNL4040_Arduino_Library.h"
VCNL4040 proximitySensor;

long startingProxValue = 0;
long deltaNeeded = 0;
boolean nothingThere = false;
String path = "/Proximity/Reading: ";
void setup()
{
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
  Serial.println("SparkFun VCNL4040 Example");

  Wire.begin(); //Join i2c bus

  if (proximitySensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1); //Freeze!
  }

  //Set the current used to drive the IR LED - 50mA to 200mA is allowed.
  proximitySensor.setLEDCurrent(200); //For this example, let's do max.

  //The sensor will average readings together by default 8 times.
  //Reduce this to one so we can take readings as fast as possible
  proximitySensor.setProxIntegrationTime(8); //1 to 8 is valid

  //Take 8 readings and average them
  for(byte x = 0 ; x < 8 ; x++)
  {
    startingProxValue += proximitySensor.getProximity();
  }
  startingProxValue /= 8;

  deltaNeeded = (float)startingProxValue * 0.05; //Look for 5% change
  if(deltaNeeded < 5) deltaNeeded = 5; //Set a minimum
}

void loop()
{
  unsigned int proxValue = proximitySensor.getProximity(); 

  Serial.print("Prox: ");
  Serial.print(proxValue);
  Serial.print(" ");

  //Let's only trigger if we detect a 5% change from the starting value
  //Otherwise, values at the edge of the read range can cause false triggers
  if(proxValue > (startingProxValue + deltaNeeded))
  {
    Serial.print("Something is there!");
    Firebase.setString(firebaseData, path, "True");
    nothingThere = false;
  }
  else
  {
    if(nothingThere == false) Serial.print("I don't see anything");
    Firebase.setString(firebaseData, path, "False");
    nothingThere = true;
  }

  Serial.println();
  delay(10);
}
