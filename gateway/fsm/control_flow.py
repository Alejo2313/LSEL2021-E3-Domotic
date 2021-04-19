from transitions.extensions import GraphMachine as Machine
from http_client import http_client
from lsel_mqttclient import mqtt_publisher
from lsel_mqttclient import mqtt_subscriber
from config import config_read
import sys
import time
import json

class Control_flow (object):
    """This class contains a fsm instantiation, and clients for http and mqtt. 

    Args:
        object (object): generic object

    Raises:
        ValueError: raised when name is empty

    Returns:
        Control_flow object: object ready to process http and mqtt responses.
    """

    states = ['CONFIG','IDLE','PROCESS_DATA','PROCESS_ORDER']

    transitions=[
        {'trigger' : 'start', 'source' : 'CONFIG',
            'dest' : 'IDLE', 'conditions' :['ready_to_work']},
        {'trigger' : 'new_data', 'source' : 'IDLE',
            'dest' : 'PROCESS_DATA', 'after':'send_data'},        
        {'trigger' : 'need_response', 'source' : 'PROCESS_DATA',
            'dest' : '=', 'after':'wait'},        
        {'trigger' : 'no_need_response', 'source' : 'PROCESS_DATA',
            'dest' : 'IDLE'},        
        {'trigger' : 'resp_ready', 'source' : 'PROCESS_DATA',
            'dest' : 'IDLE','conditions' :['msg_deliver'] ,'after':'send_order'},            
        {'trigger' : 'new_order', 'source' : 'IDLE',
            'dest' : 'PROCESS_ORDER', 'after':'send_order'},                
        {'trigger' : 'need_data', 'source' : 'PROCESS_ORDER',
            'dest' : '=', 'after':'wait'},                 
        {'trigger' : 'no_need_data', 'source' : 'PROCESS_ORDER',
            'dest' : 'IDLE'},                 
        {'trigger' : 'data_ready', 'source' : 'PROCESS_ORDER',
            'dest' : 'IDLE', 'after':'send_data'}
    ]


    config_params = config_read("/config/", "config.ini")

    config_params.read_config("config", "config.ini")

    broker_ip = config_params.get_broker_ip()
    server_ip = config_params.get_server_ip()
    header_json = config_params.get_header_json()
    print(header_json)
    header_json = {"Content-type": "application/json",
        "Accept" : "text/plain"}
    root_topic = str(config_params.get_root_topic())

    response = ""
    msg_json = ""
    
    def __init__(self, name):
        if name == "":
            raise ValueError("The name of the control flow must be non empty")
        self.name = name
        self.machine = Machine(model = self, states = Control_flow.states,
            initial = 'CONFIG', transitions=Control_flow.transitions)
        self.http_con = http_client("server",Control_flow.server_ip)


    def send_data(self):
        """method used after the fsm arrives to destination state.
        It launches a http request with the specified conten to the
        server.
        """

        fsm.no_need_response()
        self.http_con.do_post("/data",
        self.msg_json,self.header_json)
        print("data sent")
        print(fsm.state)

    def get_name(self):
        """getter method from retrieve object name.

        Returns:
            str: object name.
        """
        return str(self.name)
    

    def wait(self):
        """Waiting function.
        """
        print("waiting")

    def send_order(self):

        print("order sent")

    def ready_to_work(self):
        """Conditional function that checks if the connection with the 
        server is OK.

        Returns:
            Bool: True if OK, else False
        """
        res = self.http_con.do_get("/")
        print(res)
        return res == 200

    def msg_deliver(self):
        """Conditional function that checks if the content sent to the server
        has been accepted.

        Returns:
            Bool: True if OK, else False.
        """
        return self.http_con.do_post("/data",
            json.dumps(self.msg_json),self.header_json) == 200

def pack_info(gw_name, dev_id, data_type, ty, data):
    """method that packages the information to be sent to the server
    with specific format.

    Args:
        gw_name (String): Name of the gw
        dev_id (String): Device identifier
        data_type (String): data type to be casted (String = 2, Double = 1, Int = 0)
        ty (String): sensor type
        data (String): data received from device

    Raises:
        ValueError: [description]

    Returns:
        [type]: [description]
    """
    if (gw_name == "") or (dev_id == "") or (data_type == "") or (ty) or (data == ""):
        raise ValueError("All params must be non empty")
    print("puta")
    params = json.dumps(
            {
                "Gateway": gw_name,
                "Devices":[
                    {   
                        "Device":dev_id,    
                        "Sensors":[
                            {
                                "DType":data_type,
                                "Type":ty,
                                "data":data
                            }
                        ]

                    }
                ]
            }
        )
    return params

def on_message_suscriber(client, userdata, msg):
    """Processes the payload and topic received and creates a JSON with
    specific format.

    Args:
        client (client): client that sends the message.
        userdata (userdata): data from user such as Id.
        msg (Bytes): message sent by the client in Byte format.
    """
    msg_payload=str(msg.payload)
    msg_topic= str(msg.topic)
    splitted_msg_topic = msg_topic.split("-")
    splitted_base_topic = splitted_msg_topic[0].split("/")
    device_id=splitted_base_topic[len(splitted_base_topic)-1]
    Control_flow.response = splitted_msg_topic[2]
    values = {"S":2, "D": 1, "I": 0}
    try:
        content = pack_info(fsm.get_name(), device_id,
        values[splitted_msg_topic[1]], splitted_msg_topic[2],
        msg_payload.split("'")[1])
        Control_flow.msg_json = json.dumps(content)
    except:
        print("An exception occurred. Control object might not have been created.")
    
    print("JSON to send:\n"+Control_flow.msg_json)
    fsm.new_data()

fsm = Control_flow("gw1234")

while (True):
    print(fsm.root_topic)
    ctrl=input("Insert key:")
    if (ctrl=="1"):
        fsm.start()
        print(fsm.state)
        sub1=mqtt_subscriber(broker_addr=Control_flow.broker_ip, gw_name="gw1234",client_id="sub",
            subscribe_topic=fsm.root_topic,on_msg_function = on_message_suscriber)
        sub1.sub_all_connect()