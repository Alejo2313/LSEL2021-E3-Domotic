import paho.mqtt.client as mqtt
import json
import sys
import time


class mqtt_client(object):
    """[Class for constructor and broker address and gateway name getters]

    Args:
        object ([mqttclient]): [object for publisher and subscriber's instantiation]
    """
    def __init__(self, broker_addr, gw_name):
        self.broker_addr=broker_addr
        self.gw_name=gw_name

    def get_broker_addr(self):
        return str(self.broker_addr)
    
    def get_gw_name(self):
        return str(self.gw_name)
    

class mqtt_publisher(mqtt_client):
    """[mqtt-client sub-class for publishing messages to a specific topic]

    Args:
        mqtt_publisher ([mqtt_client]): [object that handles message publications]
    """
    def __init__(self, broker_addr, gw_name,client_id=None):
        super().__init__(broker_addr, gw_name)
        if client_id is None:
                print("The argument you passed as client_id is not valid. Try again!")
                exit()
        else:
            self.client_id=client_id

        self.client= mqtt.Client(client_id=client_id)

    def publish_msg(self, topic_base_id, data_t,device_t,endpoint_id,  msg):
        """[function that publishes a message to a specific topic
                eg: /casa/salon/0001-I-A-1
                    topic_base_id=/casa/salon/0001, data_t=I, endpoint_t=A, endpoint_id=1]

        Args:
            topic_base_id ([str]): [topic's base identifier]
            endpoint_id ([str]): [endpoint's identifier]
            msg ([byte]): [msg's payload to be sent]
        """
        def get_publish_topic(topic_base_id, data_t,device_t,endpoint_id): 
            """[returns stringed topic from the topic's base identifier and endpoint's identifier]

            Args:
                topic_base_id ([str]): [topic's base identifier]
                endpoint_id ([str]): [endpoint's identifier]

            Returns:
                [str]: [stringed complete topic]
            """
            content= topic_base_id + "-"+data_t +"-"+ device_t+"-" + endpoint_id
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
        if (data_t == "I"):
            data = int(float(msg))
        elif (data_t == "D"):
            data = float(msg)
        elif (data_t == "S"):
            data = str(msg)
        else:
            print("Data type not supported: " + str(data_t))
            exit()

        self.client.connect(self.get_broker_addr(), 1883,60)
        topic=get_publish_topic( topic_base_id, data_t,device_t,endpoint_id )
        self.client.publish(topic=topic, payload=data)


class mqtt_subscriber(mqtt_client):
    """[mqtt-client pub-class for publishing messages to a specific topic]

    Args:
        mqtt_subscriber ([mqtt_client]): [object that handles message subscriptions]
    """
    def __init__(self, broker_addr, gw_name,client_id=None,subscribe_topic=None, on_msg_function=None):
        super().__init__(broker_addr, gw_name)
        def on_message_suscriber_default(client, userdata, msg):
            """[Personalized on_message() callback to suit 
            the necessities of the LSEL project.]

            Args:
                client ([Object]): [the client instance for this callback]
                userdata ([String]): [the private user data as set in 
                Client() or user_data_set()]
                msg ([Byte]): [an instance of MQTTMessage. This is a 
                class with members topic, payload, qos, retain.]
            """
            print(str(msg.payload))

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

        self.client= mqtt.Client(client_id=client_id)
        self.client.on_message = self.on_msg_function

    def get_subscribe_topic(self):
        return str(self.subscribe_topic)

    def get_on_msg_function(self):
        return self.on_msg_function

    def get_subscribe_all_topic(self):
        """[function that returns the general stringed topic]

        Returns:
            [str]: [general stringed topic]
        """
        return self.get_subscribe_topic() + "/#"
    


    def sub_all_connect(self):
        """[function that subscribes to all the hanging topics from the base topic]
        """
        self.client.connect_async(self.get_broker_addr(), 1883,60)
        self.client.subscribe(topic=self.get_subscribe_all_topic(), qos=0)
        self.client.loop_start()
#        self.client.loop_forever()

    
    

# subscriptor1= mqtt_subscriber()

# subscriptor1.client.connect(subscriptor1.get_broker_addr)

# cliente1 = subscriptor1.create_client()


# p1=mqtt_publisher(broker_addr="localhost", gw_name="default_gw")
# p2=mqtt_publisher(broker_addr="localhost", gw_name="default_gw",topic_base_id="/casa/salon/0001")
# p3=mqtt_publisher(broker_addr="localhost", gw_name="default_gw",topic_base_id="/casa/salon/0001", endpoint_id="0001")
# p4=mqtt_subscriber(broker_addr="localhost", gw_name="default_gw",subscribe_topic="/casa")

# print("pruebas 1 \n")
# print("broker host of p1: " + p1.get_broker_addr())
# print("publish topic of p1: " + p1.get_publish_topic())
# print("gw name of p1: " + p1.get_gw_name())

# print("pruebas 2 \n")
# print("broker host of p2: " + p2.get_broker_addr())
# print("publish topic of p2: " + p2.get_publish_topic())
# print("gw name of p2: " + p2.get_gw_name())

# print("pruebas 3 \n")
# print("broker host of p3: " + p3.get_broker_addr())
# print("publish topic of p3: " + p3.get_publish_topic())
# print("gw name of p3: " + p3.get_gw_name())


# print("pruebas 4 \n")
# print("broker host of p4: " + p4.get_broker_addr())
# print("gw name of p4: " + p4.get_gw_name())
# print("subscribe topic of p4: " + p4.get_subscribe_topic())
# print("subscribe topic all of p4: " + p4.get_subscribe_all_topic())
