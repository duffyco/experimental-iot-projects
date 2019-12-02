import paho.mqtt.client as mqtt
import time
import nxppy

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print("Connected with result code" +str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("nfc")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print "Topic: ", msg.topic+'\nMessage: '+str(msg.payload)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.58", 1883, 60)

mifare = nxppy.Mifare()

while True:
   try:
       uid = mifare.select()
       print(uid)
       client.publish( "nfc", 456 );
   except nxppy.SelectError: 
       client.publish( "nfc", -1 );
       pass

   time.sleep( 1 )
