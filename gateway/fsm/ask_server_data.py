from transitions.extensions import GraphMachine as Machine
from http_client import http_client
from lsel_mqttclient import mqtt_publisher
from lsel_mqttclient import mqtt_subscriber
from config import config_read
import sys
import time
import json
import time

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
        {'trigger' : 'check_timeout', 'source' : 'START',
            'dest' : 'PROCESSING', 'conditions':['check_data'], 
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


    def check_data(self):
        
        if (self.t_init - self.t_ref > 5):
            self.t_ref = time.time()
            print("Data checked")
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
        res = self.http_con.do_get("/")
        print(res)
        return res == 200


fsm = ask_data("gw_temp")
initialize = 0

while (True):

    if (initialize == 0):
        ctrl=input("Insert key:")
        if (ctrl=="1"):
            fsm.start()
            print(fsm.state)
            initialize = 1

    fsm.check_timeout()
    
        