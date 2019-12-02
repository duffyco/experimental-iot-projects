import paho.mqtt.client as mqtt
import time
from nest_thermostat import Nest

delay = 5 

def run_process():
  global delay
  try:
    print "Trying..."
    n = Nest("nest@duffyco.ca", "!@#$1234QWERqwer", None, 0, "C")
    n.login()
    n.get_status()
    print "Received: " + str( n.temp_out(n.status['shared'][n.serial]['target_temperature']) )
    client.publish( "nest", n.temp_out(n.status['shared'][n.serial]['target_temperature']) );    
    if delay > 5 : 
       delay -= 5
  except: 
    print "Backoff"
    delay += 5 

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, rc):
    print("Connected with result code" +str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("nest")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print "Topic: ", msg.topic+'\nMessage: '+str(msg.payload)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.58", 1883, 60)

time.sleep( 5 )
print "Starting"

while True:
    run_process()
    print( "Delay: " + str( delay ) );
    time.sleep( delay )
