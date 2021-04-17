#include <TinyGPS++.h>
#include <ArduinoJson.h>

#define HOME_LAT 23.45
#define HOME_LNG 91.18
static const uint32_t GPSBaud = 9600;
static const uint32_t COMPORTBaud = 115200;
static const uint32_t ESPBaud = 9600;
float latitude, longitude, distance, alt;
uint16_t flag = 0;

TinyGPSPlus gps;


void setup(){
  Serial1.begin(COMPORTBaud); // for Debugging
  while (!Serial1) continue;
  Serial2.begin(GPSBaud);   // GPS Serial
  while (!Serial2) continue;
  Serial3.begin(ESPBaud);   // GPS Serial
  while (!Serial3) continue;
  Serial1.println("Setup: ------------------------------> DONE");

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
        Serial1.print(F(","));
        Serial1.print(alt, 6);
        Serial1.print(F(","));
        Serial1.println(distance, 6);

        flag++; 

        if (flag == 50){
          flag = 0;

          StaticJsonDocument<400> doc;
          doc["latitude"] = latitude;
          doc["longitude"] = longitude;
          doc["altitude"] = alt;
          doc["distance"] = distance;
          serializeJson(doc, Serial3);
          Serial1.println("JSON msg sent");
        }


      } else {
        Serial1.println("INVALID");
      }

    } 
  }
}
