#include<Arduino.h>
#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <LiquidCrystal_I2C.h>

#define API_KEY "AIzaSyA7i9AyTKUHuU8XOQxfpfuEhTOQUX9Edb8"
#define DATABASE_URL "https://selfwateringsystem-16930-default-rtdb.europe-west1.firebasedatabase.app/"
#define WIFI_SSID "your network"
#define WIFI_PASSWORD "your password"

unsigned long sendDataPrevMillis = 0;
bool signupOK=false;
const int soilMoisturePin = A0; // Pin for capacitive soil moisture
const int relayPin = D0;         // Pin for relay module
const int drySoil = 730;  // Minimum value for dry soil
const int wetSoil = 309;  // Maksimum value for wet soil
LiquidCrystal_I2C lcd(0x27,16,2);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  lcd.begin();
  lcd.backlight();

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting to WI-Fi");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP:");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key=API_KEY;
  config.database_url=DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
   int sensorValue=analogRead(soilMoisturePin);
   int procenatVlaznosti=map(sensorValue,wetSoil,drySoil,100,0);

   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setInt(&fbdo, "IoT/procenatVlaznosti", procenatVlaznosti);
    if (Firebase.RTDB.setInt(&fbdo, "IoT/vlaznostTla", sensorValue)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (sensorValue > 600) { 
    digitalWrite(relayPin, LOW); // Pump ON

    lcd.setCursor(0, 0);
    lcd.print("Vlaznost: "); 
    lcd.print(procenatVlaznosti); 
    lcd.print("%   ");
    lcd.setCursor(0, 1);
    lcd.print("Motor ON       "); 
    Serial.print("Tlo je suho, vrijednost vlažnosti tla: ");
    Serial.println(sensorValue);
    Serial.print("Procenat vlažnosti: ");
    Serial.print(procenatVlaznosti);
    Serial.print("% ");
    Serial.println("Pump On");
    Firebase.RTDB.setInt(&fbdo, "IoT/stanjePumpe", 1);
  } else if(sensorValue < 500){
    digitalWrite(relayPin, HIGH); // Pump OFF

    lcd.setCursor(0, 0);
    lcd.print("Vlaznost: "); 
    lcd.print(procenatVlaznosti); 
    lcd.print("%   ");

    lcd.setCursor(0, 1);
    lcd.print("Motor OFF      ");

    Serial.print("Tlo je mokro, vrijednost vlažnosti tla: ");
    Serial.println(sensorValue);
    Serial.print("Procenat vlažnosti: ");
    Serial.print(procenatVlaznosti);
    Serial.print("% ");
    Serial.println("Pump Off");
    Firebase.RTDB.setInt(&fbdo, "IoT/stanjePumpe", 0);
  }
  }
  delay(1000);
}