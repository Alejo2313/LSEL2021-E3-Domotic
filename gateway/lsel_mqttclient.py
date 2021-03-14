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

import paho.mqtt.client as mqtt
import json
import sys
import time

def on_message_suscriber_default(client, userdata, msg):
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
    print(str(msg.payload))



class mqtt_client(object):
    """[Topic form topic_base_id-data_t-endpoint_t-endpoint_id 
            eg: /casa/salon/0001-I-A-1
                topic_base_id=/casa/salon/0001, data_t=I, endpoint_t=A, endpoint_id=1
        ]
    
    Args:
        object ([type]): [description]
    """
    def __init__(self, broker_addr, gw_name):
        self.broker_addr=broker_addr
        self.gw_name=gw_name

    def get_broker_addr(self):
        return str(self.broker_addr)
    
    def get_gw_name(self):
        return str(self.gw_name)
    

class mqtt_publisher(mqtt_client):
    def __init__(self, broker_addr, gw_name,client_id=None):
        super().__init__(broker_addr, gw_name)
        if client_id is None:
                print("The argument you passed as client_id is not valid. Try again!")
                exit()
        else:
            self.client_id=client_id

        self.client=client= mqtt.Client(client_id=client_id)
    

    def publish_msg(self, topic_base_id, endpoint_id, msg):
        def get_publish_topic(self,topic_base_id,endpoint_id): 
            content= topic_base_id + "-I-A-" + endpoint_id
            return str(content)

        if msg== None:
                print("The message you passed is not valid. Try again!")
                exit()        
        if topic_base_id== None:
                print("The topic_base_id you passed is not valid. Try again!")
                exit()
        if endpoint_id== None:
                print("The endpoint_id you passed is not valid. Try again!")
                exit()

        self.client.connect(self.get_broker_addr(), 1883,60)
        self.client.publish(topic=get_publish_topic(topic_base_id, endpoint_id), payload=msg)





class mqtt_subscriber(mqtt_client):
    def __init__(self, broker_addr, gw_name,client_id=None,subscribe_topic=None, on_msg_function=None):
        super().__init__(broker_addr, gw_name)

        if on_msg_function is None:
            self.on_msg_function=on_message_suscriber_default
        else:
            if callable(on_msg_function):
                self.on_msg_function=on_msg_function
            else:
                print("The argument you passed as on_msg_function is not a function. Try again!")
                exit()
        
        if client_id is None:
                print("The argument you passed as client_id is not valid. Try again!")
                exit()
        else:
            self.client_id=client_id

        if subscribe_topic is None:
            self.subscribe_topic=""
        else:
            self.subscribe_topic=subscribe_topic

        self.client=client= mqtt.Client(client_id=client_id)
        self.client.on_message = on_msg_function

    def get_subscribe_topic(self):
        return str(self.subscribe_topic)

    def get_subscribe_all_topic(self):
        return self.get_subscribe_topic() + "/#"
    
    def get_on_msg_function(self):
        return self.on_msg_function

    def sub_all_connect(self):
        self.client.connect(self.get_broker_addr(), 1883,60)
        self.client.subscribe(topic=self.get_subscribe_all_topic(), qos=0)
        self.client.loop_forever()

    
    

subscriptor1= mqtt_subscriber()

subscriptor1.client.connect(subscriptor1.get_broker_addr)

cliente1 = subscriptor1.create_client()


p1=mqtt_publisher(broker_addr="localhost", gw_name="default_gw")
p2=mqtt_publisher(broker_addr="localhost", gw_name="default_gw",topic_base_id="/casa/salon/0001")
p3=mqtt_publisher(broker_addr="localhost", gw_name="default_gw",topic_base_id="/casa/salon/0001", endpoint_id="0001")
p4=mqtt_subscriber(broker_addr="localhost", gw_name="default_gw",subscribe_topic="/casa")

print("pruebas 1 \n")
print("broker host of p1: " + p1.get_broker_addr())
print("publish topic of p1: " + p1.get_publish_topic())
print("gw name of p1: " + p1.get_gw_name())

print("pruebas 2 \n")
print("broker host of p2: " + p2.get_broker_addr())
print("publish topic of p2: " + p2.get_publish_topic())
print("gw name of p2: " + p2.get_gw_name())

print("pruebas 3 \n")
print("broker host of p3: " + p3.get_broker_addr())
print("publish topic of p3: " + p3.get_publish_topic())
print("gw name of p3: " + p3.get_gw_name())


print("pruebas 4 \n")
print("broker host of p4: " + p4.get_broker_addr())
print("gw name of p4: " + p4.get_gw_name())
print("subscribe topic of p4: " + p4.get_subscribe_topic())
print("subscribe topic all of p4: " + p4.get_subscribe_all_topic())
