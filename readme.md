# GPS location plot Google Maps API

- GPS_MQTT 
	-
    	- GPS_MQTT directory contains the firmware for ESP8266
    	- Change the MQTT host address, usename and password.
    	- You can use any MQTT broker such as adafruit_io or mosquitto CLI

- djangoServer
	-	
    	- Contains the python scripts 
    	- install paho-mqtt and gmplot
    	- install postgres 
    	- check the requirement.txt file to setup the virtual environment
    	- run django server
- djangoServer/MapHandler/MQTT_filewrite/MQTTRec.py
	-
	- run in a separate terminal/power shell to listen to the messages coming from ESP8266 and add them to the database
