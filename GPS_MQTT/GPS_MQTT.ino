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

#define     HOST    "192.168.1.9"
#define     PORT    1883
#define     USRNAME "sakib"
#define     KEY     "hd85512b"

/***********************************************************************************/
/***************************** MQTT instance ***************************************/
/***********************************************************************************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, HOST, PORT, USRNAME, KEY);


/****************************ls*******************************************************/
/****************************** Topic **********************************************/
/***********************************************************************************/

Adafruit_MQTT_Publish _gps_lat = Adafruit_MQTT_Publish(&mqtt, "/gps_lat");
Adafruit_MQTT_Publish _gps_lng = Adafruit_MQTT_Publish(&mqtt, "/gps_lng");


/***********************************************************************************/
/***************************** GPS Object ******************************************/
/***********************************************************************************/


static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

float latitude, longitude;

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
        if (gps.location.isValid())
        {
            latitude = gps.location.lat();
            longitude = gps.location.lng();
            Serial.print(latitude, 6);
            Serial.print(F(","));
            Serial.println(longitude, 6);


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
            Serial.println(" ");
            Serial.println(" ");
            Serial.println(" ");
            Serial.println(" ");
            Serial.println(" ");
        }
        else
        {
            Serial.println(F("GPS --------------------> FAILED!!!"));
        }
    }

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        while(true);
    }
    }

    
}