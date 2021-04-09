import psycopg2
from paho.mqtt import client as mqtt_client


broker = 'localhost'
port = 1883
topic = "sakib/feeds/gpslatlng/csv"
client_id = '#test_ID_remote_admin_2'
username = 'sakib'
password = 'enteryourpass'
latitude = 0
longitude = 0
i = 0


conn = psycopg2.connect(
    database="sakibdb",
    user = "sakib",
    password="hd85512b",
    host="localhost",
    port="5432"
)
print("connected")
cur = conn.cursor()


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


def subscribe(client: mqtt_client):
    def on_message(client, userdata, msg):
        message = msg.payload.decode()

        x = message.split(",")
        latitude = x[1]
        longitude = x[2]

        cur.execute ("INSERT INTO map_loc(index,lats,lngs)values("+str(100)+","+latitude+","+longitude+")")
        conn.commit()
        print(f"Received `{message}` from topic")



    client.subscribe(topic)
    client.on_message = on_message


def run():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()


if __name__ == '__main__':
    run()