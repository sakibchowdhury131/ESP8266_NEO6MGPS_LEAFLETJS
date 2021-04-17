#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFi.h>
#include <SoftwareSerial.h>

/************************************************************************************/
/***************************** Defining Wifi ****************************************/
/************************************************************************************/

#define     SSID    "UnknownDevice"
#define     PASS    "sakibtuhinnitu"

/************************************************************************************/
/***************************** MQTT params ******************************************/
/************************************************************************************/

#define     HOST    "192.168.1.4"
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

Adafruit_MQTT_Publish _gps_lat_lng = Adafruit_MQTT_Publish(&mqtt, USRNAME"/feeds/gpslatlng/csv");


/***********************************************************************************/
/***************************** STM32 Comm ******************************************/
/***********************************************************************************/

static const int RXPin = 13, TXPin = 15;
static const uint32_t STM32Baud = 115200;
char gpsbuffer[30]; 

SoftwareSerial ss(RXPin, TXPin);


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
    ss.begin(STM32Baud);

    delay (10); 
    Serial.println("Setup Started... ");

    Serial.print("connecting to "); Serial.println(SSID);
    
    WiFi.begin(SSID, PASS); 
    Serial.println("error");
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
        
        Serial.print("Sending GPS Data -------------------> ");
        if (! _gps_lat_lng.publish(ss.read())) {
            Serial.println("FAILED");
        } else {
            Serial.println("OK");
        }
    }
}
