from paho.mqtt import client as mqtt_client
import gmplot

##################################################################################################
########################################### GMPLOT ###############################################
##################################################################################################

gmap3 = gmplot.GoogleMapPlotter(23.45, 91.18, 13)
latitude = 23.45
longitude = 91.18

##################################################################################################
############################################## MQTT Params #######################################
##################################################################################################

broker = '192.168.1.9'
port = 1883
topic = "#"
client_id = '#test_ID_remote_admin_2'
username = 'sakib'
password = 'enteryourpass'

##################################################################################################
############################################## Connection to MQTT ################################
##################################################################################################

def connect_mqtt() -> mqtt_client:
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client

####################################################################################################
####################################### Subscribe ##################################################
####################################################################################################

def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        messageTemp = msg.payload.decode()
        topicTemp = msg.topic

        latitude = 23.45
        longitude = 91.18
        

        if (topicTemp == "/gps_lat"):
            latitude = float(messageTemp)
            print(f"Received `{latitude}` from `{topicTemp}` topic")
        if (topicTemp == "/gps_lng"):
            longitude = float(messageTemp)
            print(f"Received `{longitude}` from `{topicTemp}` topic")
    
        lists = zip(*[(latitude,longitude)])
        gmap3.plot(*lists, 'cornflowerblue', edge_width = 3.0)
        gmap3.apikey = 'AIzaSyAIB5L-3V1u0iA-11Sk7PafVv6JgV11Pkg'
        gmap3.draw(r'map.html')
        
        

    client.subscribe(topic)
    client.on_message = on_message

####################################################################################################

def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

if __name__ == '__main__':
    run()
