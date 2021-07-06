#if defined(ESP32)
#include <WiFi.h>
#include <time.h>
#define USE_ARDUINO_INTERRUPTS true  
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "polopolo"
#define WIFI_PASSWORD "qwertyytrewq"

/* 2. Define the API Key */
#define API_KEY "AIzaSyB25fJSi_15-W7mLPiJiXgyKorDjYwIHUI"

/* 3. Define the RTDB URL */
#define DATABASE_URL "elated-capsule-249919.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "test@gmail.com"
#define USER_PASSWORD "########"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int timezone = 5 * 3600;
int dst = 0;
int count = 0;


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

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the user sign in credentials */
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

    configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK"); 
 
}

void loop()
{
//    time_t now = time(nullptr);
//    struct tm* p_tm = localtime(&now);
//    Serial.print(p_tm->tm_mday);
//    Serial.print("/");
//    Serial.print(p_tm->tm_mon + 1);
//    Serial.print("/");
//    Serial.print(p_tm->tm_year + 1900);
    
//      Serial.print(" ");

//    Serial.print(p_tm->tm_hour);
//    Serial.print(":");
//    Serial.print(p_tm->tm_min);
//    Serial.print(":");
//    Serial.println(p_tm->tm_sec);
  
//  delay(1000);

//  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
//{

        
        sendDataPrevMillis = millis();

        FirebaseJson json;
        json.add("time", "om");
        json.add("value", "om");
    
        Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/test/push", json) ? "ok" : fbdo.errorReason().c_str());
    
        json.set("time",count);
        json.set("value",(analogRead(A0)/10));
        
        Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/test/push/" + fbdo.pushName()), json) ? "ok" : fbdo.errorReason().c_str());
        delay(5000);
    
        count = count+5;
//  }
}
