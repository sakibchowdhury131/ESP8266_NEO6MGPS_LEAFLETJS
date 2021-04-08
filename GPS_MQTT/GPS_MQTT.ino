#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

/************************************************************************************/
/***************************** Defining Wifi ****************************************/
/************************************************************************************/

#define     SSID    "repeater"
#define     PASS    "sakibtuhinnitu"

/************************************************************************************/
/***************************** MQTT params ******************************************/
/************************************************************************************/

#define     HOST    "io.adafruit.com"
#define     PORT    1883
#define     USRNAME "bueteeeproject"
#define     KEY     "aio_TCbI50VpU0fPx2jDnPVeVg50NRSr"

/***********************************************************************************/
/***************************** MQTT instance ***************************************/
/***********************************************************************************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, HOST, PORT, USRNAME, KEY);


/****************************ls*******************************************************/
/****************************** Topic **********************************************/
/***********************************************************************************/

Adafruit_MQTT_Publish _gps_lat = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslat");
Adafruit_MQTT_Publish _gps_lng = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslng");
Adafruit_MQTT_Publish _gps_lat_lng = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslatlng/csv");


/***********************************************************************************/
/***************************** GPS Object ******************************************/
/***********************************************************************************/
#define HOME_LAT 23.45
#define HOME_LNG 91.18
uint8_t flag = 0;

static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

float latitude, longitude, distance, alt;

/***********************************************************************************/
/*********************************** MQTT Connection *******************************/
/***********************************************************************************/

void MQTT_connect(){
    int8_t ret;
    if (mqtt.connected()) {
    return;
    }
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
}


void setup(){
    Serial.begin(115200);
    ss.begin(GPSBaud);


    delay (10); 
    Serial.println("Setup Started... ");

    


    Serial.print("connecting to "); Serial.println(SSID);
    WiFi.begin(SSID, PASS); 
    while (WiFi.status() != WL_CONNECTED){
        delay(500); 
        Serial.print(".");
    }

    Serial.println(" ");
    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());

    Serial.println("Setup Done ... ");

}


void loop(){
    MQTT_connect();


    while (ss.available()){
    if (gps.encode(ss.read())){
        Serial.println("GPS Serial read ----------------> OK");
        if (gps.location.isValid())
        {
            Serial.println("GPS Serial data ----------------> VALID");
            flag++;
            Serial.print("flag: ");Serial.println(flag);

            if (flag == 100){
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                alt = gps.altitude.feet();
                distance = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),HOME_LAT, HOME_LNG);  //Query Tiny GPS to Calculate Distance to Home

                Serial.print(latitude, 6);
                Serial.print(F(","));
                Serial.print(longitude, 6);
                Serial.print(F(","));
                Serial.print(alt, 6);
                Serial.print(F(","));
                Serial.println(distance, 6);






                char gpsbuffer[30];                         // Combine Latitude, Longitude, Altitude into a buffer of size X
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


                Serial.println(" ");
                Serial.println(" ");
                flag = 0;
            }
        }
        else
        {
            Serial.println(F("GPS --------------------> FAILED!!!"));
        }
    }

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        //while(true);
    }
    }

    
}