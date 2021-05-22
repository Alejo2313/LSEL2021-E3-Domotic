from transitions.extensions import GraphMachine as Machine
from http_client import http_client
from lsel_mqttclient import mqtt_publisher
from lsel_mqttclient import mqtt_subscriber
from config import config_read
import sys
import time
import json
import time
import requests
import paho.mqtt.client as mqtt

session = requests.Session()

class ask_data (object):
    """This class contains a fsm instantiation, and clients for http and mqtt. 

    Args:
        object (object): generic object

    Raises:
        ValueError: raised when name is empty

    Returns:
        Control_flow object: object ready to process http and mqtt responses.
    """

    states = ['IDLE','START','PROCESSING']

    transitions=[
        {'trigger' : 'start', 'source' : 'IDLE',
            'dest' : 'START', 'conditions' :['ready_to_work']},
        {'trigger' : 'tout', 'source' : 'START',
            'dest' : 'PROCESSING', 'conditions':['check_tout'], 
            'after' : 'process_data'},        
        {'trigger' : 'is_processed', 'source' : 'PROCESSING',
            'dest' : 'START', 'conditions':['data_processed'], 
            'after' : 'send_mqtt'}        
    ]


    config_params = config_read("config", "config.ini")

    config_params.read_config(config_params.get_path(), config_params.get_f_name())

    broker_ip = config_params.get_broker_ip()
    server_ip = config_params.get_server_ip()
    header_json = json.loads(config_params.get_header_json())
    root_topic = config_params.get_root_topic()
    response = ""
    msg_json = ""
    t_ref = time.time()
    t_init = time.time()
    
    def __init__(self, name):
        if name == "":
            raise ValueError("The name of the control flow must be non empty")
        self.name = name
        self.machine = Machine(model = self, states = ask_data.states,
            initial = 'IDLE', transitions=ask_data.transitions)
        self.http_con = http_client("server",ask_data.server_ip)


    def check_tout(self):
        """Checks if time elapsed

        Returns:
            Boolean: True if elapsed, else False
        """
        
        if (self.t_init - self.t_ref > 0.25):
            self.t_ref = time.time()
            print("Time elapsed")
            print(fsm.state)
            return True
        else:
            self.t_init = time.time()
            return False


    def get_name(self):
        """getter method from retrieve object name.

        Returns:
            str: object name.
        """
        return str(self.name)
    

    def process_data(self):
        print("Data processed")
#        res = json.loads(self.get_content().decode('utf-8'))
        res = self.get_content()
        if (len(res) == 0):
            print("Nothing to do")
        else:
            for data in res:
                print(data)
                if (data["SensorType"] == 2):
                    self.prepare_msg(fsm.root_topic,data["Device"],data["SensorType"],str(int(data["Data"][1:],16)))
                else:
                    self.prepare_msg(fsm.root_topic,data["Device"],data["SensorType"],data["Data"])
        fsm.is_processed()

    def data_processed(self):

        print("Data processed")
        return True

    def send_mqtt(self):

        print("mqtt package sent")

    def ready_to_work(self):
        """Conditional function that checks if the connection with the 
        server is OK.

        Returns:
            Bool: True if OK, else False
        """
        res = self.http_con.do_get_status("/")
        print(res)
        return res == 200

    def get_content(self):
        gw_id = {
            "Gateway" : "poc1"
        }
        r = session.get('http://40.114.216.24:80/data', json=gw_id)
        print(r.status_code)
        print(r.json())
        return r.json()

    def prepare_msg(self, base_id, dev_id, dev_ty, pld):
#        topic = base_id + "/" dev_id + "-I" + "-" + str(dev_ty) + "-O"
        client= mqtt.Client(client_id="tpm")
        client.connect(fsm.broker_ip, 1883,60)
        topic= base_id + "/" + dev_id + "-I" + "-" + str(dev_ty) + "-O"
        print(topic)
        client.publish(topic=topic, payload=pld)

fsm = ask_data("poc1")
initialize = 0

while (True):

    if (initialize == 0):
        ctrl=input("Insert key:")
        if (ctrl=="1"):
            fsm.start()
            print(fsm.state)
            initialize = 1

    fsm.tout()
    
        