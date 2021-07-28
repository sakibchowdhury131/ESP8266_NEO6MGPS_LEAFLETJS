#include <ArduinoJson.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFi.h>
double latitude, longitude, distance, alt;
char gpsbuffer[40];



/************************************************************************************/
/***************************** Defining Wifi ****************************************/
/************************************************************************************/

#define     SSID    "NoNetwork"
#define     PASS    "sakibtuhinnitu"





/************************************************************************************/
/***************************** MQTT params ******************************************/
/************************************************************************************/

#define     HOST    "test.mosquitto.org"
#define     PORT    1883
#define     USRNAME "sakib"
#define     KEY     "nopass"





/***********************************************************************************/
/***************************** MQTT instance ***************************************/
/***********************************************************************************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, HOST, PORT, USRNAME, KEY);

/***********************************************************************************/
/****************************** Topic **********************************************/
/***********************************************************************************/

Adafruit_MQTT_Publish _gps_lat = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslat");
Adafruit_MQTT_Publish _gps_lng = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslng");
Adafruit_MQTT_Publish _gps_lat_lng = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslatlng/csv");


/***********************************************************************************/
/*********************************** MQTT Connection *******************************/
/***********************************************************************************/

void MQTT_connect(){
    if (mqtt.connected()) {
    return;
    }

    int8_t ret;
    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);  // wait 5 seconds
        retries--;
        if (retries == 0) {
            // basically die and wait for WDT to reset me
            while (1);
        }
    }
    Serial.println("MQTT Connected!");
    StaticJsonDocument<400> temp;
    temp["WiFi_status"] = 1;
    temp["MQTT_status"] = 1;
    serializeJson(temp, Serial2);
    Serial.println("Status sent"); 
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);

  Serial.println("Setup Started... ");

    


  Serial.print("connecting to "); Serial.println(SSID);
  WiFi.begin(SSID, PASS); 
  while (WiFi.status() != WL_CONNECTED){
      delay(500); 
      Serial.print(".");
  }

  Serial.println(" ");
  Serial.println("WiFi connected");
  StaticJsonDocument<400> temp;
  temp["WiFi_status"] = 1;
  temp["MQTT_status"] = 0;
  serializeJson(temp, Serial2);
  Serial.println("Status sent"); 
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  Serial.println("Setup Done ... ");

}

void loop() {
  // put your main code here, to run repeatedly:
  MQTT_connect();
  if (Serial2.available()){
    StaticJsonDocument<500> doc;
    DeserializationError err = deserializeJson(doc, Serial2);


    if (err == DeserializationError::Ok){
      latitude = doc["latitude"].as<double>();
      longitude = doc["longitude"].as<double>();
      alt = doc["altitude"].as<double>();
      distance = doc["distance"].as<double>();

      Serial.print("Latitude: "); Serial.println(latitude);
      Serial.print("Longitude: "); Serial.println(longitude);
      Serial.print("Altitude: "); Serial.println(alt);
      Serial.print("Distance: "); Serial.println(distance);


      char gpsbuffer[30];
      char *p = gpsbuffer;
      dtostrf(distance, 3, 4, p);         // Convert Distance to Home to a String Variable and add it to the buffer
      p += strlen(p);
      p[0] = ','; p++;                 


      dtostrf(latitude, 3, 6, p);                   // Convert GPSlat(latitude) to a String variable and add it to the buffer
      p += strlen(p);
      p[0] = ','; p++;


      dtostrf(longitude, 3, 6, p);                   // Convert GPSlng(longitude) to a String variable and add it to the buffer
      p += strlen(p);
      p[0] = ','; p++;  


      dtostrf(alt, 2, 1, p);                   // Convert GPSalt(altimeter) to a String variable and add it to the buffer
      p += strlen(p);
                                                                  
      p[0] = 0;

      Serial.println(gpsbuffer);


      Serial.println("sending GPS co-ordinates...");
      if (! _gps_lat.publish(latitude)) {
          Serial.println(F("GPS_latitude--------------------- FAILED"));
      } else {
          Serial.println(F("GPS_latitude--------------------- OK"));
      }

      if (! _gps_lng.publish(longitude)) {
          Serial.println(F("GPS_longitude--------------------- FAILED"));
      } else {
          Serial.println(F("GPS_longitude--------------------- OK"));
      }

      if (! _gps_lat_lng.publish(gpsbuffer)) {
          Serial.println(F("GPS_data--------------------- FAILED"));
      } else {
          Serial.println(F("GPS_data--------------------- OK"));
      }

  }

}
}
