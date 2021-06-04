#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ESP8266HTTPClient.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "AnotherIFTTTWebhook.h"

#define servoOn D8
#define servoOff D7
#define DHT11PIN D6
#define TRIG D2
#define ECHO D1
#define buzzer D4
#define button D3


#define WLAN_SSID       "INS"             // Your SSID  //check
#define WLAN_PASS       ""        // Your password  //check

/************************* Adafruit.io Setu p *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "EnochOf"            // Replace it with your username  //check
#define AIO_KEY         ""   // Replace with your Project Auth Key  //check

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

HTTPClient mClient;

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/LedControlFeed"); // FeedName  //check

Servo left, right;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  //pinMode(servoOn, OUTPUT);
  //pinMode(servoOff, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  mqtt.subscribe(&Light1);

  pinMode(buzzer, OUTPUT);                    
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  left.attach(servoOff);
  right.attach(servoOn);

  
}

void loop() {

  MQTT_connect();

  //cho();
  delay(20);

  Adafruit_MQTT_Subscribe *subscription = mqtt.readSubscription(1);

     if (subscription == &Light1) {
          Serial.print("Got: ");
          Serial.println((char *)Light1.lastread);  
          
          if(*Light1.lastread == '0'){
             leftR();
          }
          else if(*Light1.lastread == '1'){
            rightR();
          }
          else security = 0;
     }

  if(digitalRead(button) == 0){
    delay(1000);
    security ^= 1;
  }

  if(millis() - lastMs >= 15000){
    lastMs = millis();

    int readTemp = 0;
    int readHumid = 0;
    //readDHT11(&readTemp, &readHumid);
    
    char allBuf[200];
    Serial.printf("Temp:%d, Humid:%d\r\n",readTemp, readHumid);

    //snprintf(allBuf, sizeof(allBuf), "http://api.thingspeak.com/update?api_key=A1QBNUU4080IBIIZ&field1=%d&field2=%d", readTemp, readHumid);

    /*
    mClient.begin(allBuf);
    mClient.GET();
    mClient.getString();
    mClient.end();
    */
  }

  delay(3);
}
