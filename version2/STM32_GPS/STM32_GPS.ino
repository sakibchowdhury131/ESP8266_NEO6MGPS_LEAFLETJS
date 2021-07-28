#include <TinyGPS++.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>


static const uint32_t GPSBaud = 9600;
static const uint32_t COMPORTBaud = 115200;
static const uint32_t ESPBaud = 115200;
float latitude, longitude, distance, alt;
uint16_t flag = 0;

TinyGPSPlus gps;
const int rs = PA6, en = PA5, d4 = PA4, d5 = PA1, d6 = PA0, d7 = PC15; //STM32 Pins to which LCD is connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize the LCD


void setup(){
  Serial1.begin(COMPORTBaud); // for Debugging
  while (!Serial1) continue;
  Serial2.begin(GPSBaud);   // GPS Serial
  while (!Serial2) continue;
  Serial3.begin(ESPBaud);   // GPS Serial
  while (!Serial3) continue;
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setup Done");
  Serial1.println("Setup: ------------------------------> DONE");
  delay(1000);

}

void loop(){
  while (Serial2.available()){
    if (gps.encode(Serial2.read())){
      Serial1.println("GPS Encoding --------------------> Done");
      Serial1.print("GPS location --------------------> ");
      if (gps.location.isValid()){
        Serial1.println("VALID");
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        alt = gps.altitude.feet();
        distance = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),HOME_LAT, HOME_LNG);  //Query Tiny GPS to Calculate Distance to Home


        Serial1.print(latitude, 6);
        Serial1.print(F(","));
        Serial1.print(longitude, 6);
        

        flag++; 

        if (flag == 50){
          flag = 0;

          StaticJsonDocument<400> doc;
          doc["latitude"] = latitude;
          doc["longitude"] = longitude;
          serializeJson(doc, Serial3);
          Serial1.println("JSON msg sent");
          //lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Lats: ");
          lcd.print(latitude,6);
          lcd.print("N");
          lcd.setCursor(0, 1);
          lcd.print("Lngs: ");
          lcd.print(longitude, 6);
          lcd.print("E");
        }


      } else {
        Serial1.println("INVALID");
        lcd.setCursor(0,0);
        lcd.print("Location Invalid");
      }

    } 
  }

  if (Serial3.available()){
    StaticJsonDocument<500> esp_status;
    DeserializationError err = deserializeJson(esp_status, Serial3);

    if (err == DeserializationError::Ok){
      int wifiConnected = esp_status["WiFi_status"].as<int>();
      int mqttConnected = esp_status["MQTT_status"].as<int>();

      if (wifiConnected == 1){
        lcd.setCursor(0,1);
        lcd.print("WiFi Connected!!");

        if (mqttConnected == 1){
          lcd.setCursor(0,1);
          lcd.print("MQTT Connected!!");
        } else {
          lcd.setCursor(0,1);
          lcd.print("MQTT Failed!!!");
        }
        
      } else {
        lcd.setCursor(0,1);
        lcd.print("WiFi Failed!!!");
      }
    }
  }

  
}
