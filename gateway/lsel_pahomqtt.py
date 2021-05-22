import paho.mqtt.client as mqtt
import json
import sys
import time

def get_json_from_msg (msg):
    """[function used to return a json with our data structure 
    from the message received.]

    Args:
        msg ([Byte]): [message received from the topic]
    """
    stringed_msg_payload=str(msg.payload)
    stringed_msg_topic= str(msg.topic)
    stringed_splitted_msg_topic = stringed_msg_topic.split("-")

    return json.dumps(
        {
        "gateway_name": "Nombre gateway",
        "local_id": stringed_splitted_msg_topic[0],
        "data_t": stringed_splitted_msg_topic[1],
        "src_t": stringed_splitted_msg_topic[2],
        "src_id": stringed_splitted_msg_topic[3],
        "data": stringed_msg_payload[2:len(stringed_msg_payload)-1:]
        }
    )

def on_message_suscriber(client, userdata, msg):
    """[Personalized on_message() callback to suit 
    the necessities of the LSEL project.
    
    Called when a message has been received on a topic
    that the client subscribes to and the message does
    not match an existing topic filter callback. Use
    message_callback_add() to define a callback that
    will be called for specific topic filters. 
    on_message will serve as fallback when none matched.]

    Args:
        client ([Object]): [the client instance for this callback]
        userdata ([String]): [the private user data as set in 
        Client() or user_data_set()]
        msg ([Byte]): [an instance of MQTTMessage. This is a 
        class with members topic, payload, qos, retain.]
    """
    print(get_json_from_msg(msg))

def on_connect_suscriber(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(topic="/casa/#", qos=0)


# This scritps needs a few input arguments
# Arg N1 : broker type
# Arg N2 : client mode
# Arg N3 : client id
# Arg N4 : Broker hostname
# Arg N5 : Gateway name
brokers = "mosquitto or paho-mqtt"



# Parameters definition
scriptName = "mqtt_connect.py"
if (len(sys.argv) < 4) or (len(sys.argv) < 2):
    print("Not enought input arguments, please run " + scriptName + " -h for help")
    exit()
elif sys.argv[1] == "-h":
    print("This script needs 3 input arguments: \n The first one is the broker type:  -m (mosquitto) or -pm (paho-mqtt)"
    +"\n The second one is client mode: -p (publisher) or -s (subscriber)\n third is client id\n"
    + "forth is the broker hostname to connect to\n"+ "fifth is the gateway name (not implemented)")
    exit()
else:
    # brokerType = sys.argv[1]
    # clientMode = sys.argv[2]
    # clientId   = sys.argv[3]
    # brokerHost = sys.argv[4]
    # gwName = sys.argv[5]

    clientMode = sys.argv[1]
    clientId   = sys.argv[2]
    brokerHost = sys.argv[3]

if clientMode == "-p":
    client = mqtt.Client(client_id=clientId)
    client.connect(brokerHost, 1883, 60)
    for i in range(1,10):
        client.publish(topic="/casa/salon/0001-I-S-1", payload="hola klk la gente"+ str(i))
        time.sleep(2)
    client.loop_forever()
elif clientMode == "-s":
    client = mqtt.Client(client_id=clientId)
    client.on_connect = on_connect_suscriber
    client.on_message = on_message_suscriber
    client.connect(brokerHost, 1883, 60)
    client.loop_forever()

