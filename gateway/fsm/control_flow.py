from transitions.extensions import GraphMachine as Machine
from http_client import http_client
from lsel_mqttclient import mqtt_publisher
from lsel_mqttclient import mqtt_subscriber
import sys
import time
import json

class Control_flow (object):
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

    server_ip = "40.114.216.24"
#    broker_ip = "192.168.1.41"
    broker_ip = "localhost"
    header_json = {"Content-type": "application/json",
        "Accept" : "text/plain"}
    response = ""
    msg_json = ""
    
    def __init__(self,name):
        if name == "":
            raise ValueError("The name of the control flow must be non empty")
        self.name = name
        self.machine = Machine(model = self, states = Control_flow.states,
            initial = 'CONFIG', transitions=Control_flow.transitions)
        self.http_con = http_client("server",Control_flow.server_ip)

    def send_data(self):
#        if (Control_flow.response == "1"): #Revisar 
        fsm.no_need_response()
        print("Puta")
        self.http_con.do_post("/data",
        self.msg_json,self.header_json)
        # else:
        #     print("Waiting for order")
        #     time.sleep(5)
        #     fsm.need_response()

        print("data sent")
        print(fsm.state)

    def get_name(self):
        return str(self.name)

    def wait(self):
        print("waiting")

    def send_order(self):
        print("order sent")

    def ready_to_work(self):
        res = self.http_con.do_get("/")
        print(res)
        return res == 200

    def msg_deliver(self):
        return self.http_con.do_post("/data",
            json.dumps(self.msg_json),self.header_json) == 200

def on_message_suscriber(client, userdata, msg):
    print("pm")
    msg_payload=str(msg.payload)
    msg_topic= str(msg.topic)
    splitted_msg_topic = msg_topic.split("-")
    splitted_base_topic = splitted_msg_topic[0].split("/")
    device_id=splitted_base_topic[len(splitted_base_topic)-1]
    Control_flow.response = splitted_msg_topic[2]
    values = {"S":2, "D": 1, "I": 0}
    try:
        Control_flow.msg_json = json.dumps(
            {
                "Gateway": fsm.get_name(),
                "Devices":[
                    {   
                        "Device":device_id,    
                        "Sensors":[
                            {
                                "DType":values[splitted_msg_topic[1]],
                                "Type":splitted_msg_topic[2],
                                "data":msg_payload.split("'")[1]
                            }
                        ]

                    }
                ]
            }
        )
    except:
        print("An exception occurred. Control object might not have been created.")
    
    print("JSON to send:\n"+Control_flow.msg_json)
    fsm.new_data()

fsm = Control_flow("gw1234")

while (True):
    ctrl=input("Insert key:")
    if (ctrl=="1"):
        fsm.start()
        print(fsm.state)
        sub1=mqtt_subscriber(broker_addr=Control_flow.broker_ip, gw_name="gw1234",client_id="sub",
            subscribe_topic="/home",on_msg_function = on_message_suscriber)
        sub1.sub_all_connect()
        
#    elif (ctrl=="2"):
        # print("New data received: " + str(params))
        # fsm.new_data()
        # print(fsm.state)
        # if (params["back"] == "0"):
        #     fsm.no_need_response()
        # else:
        #     fsm.need_response()
        #     fsm.resp_ready()
        # print(fsm.state)
    # elif (ctrl=="3"):
    #     fsm.need_response()
    #     print(fsm.state)
    # elif (ctrl=="4"):
    #     fsm.no_need_response()
    #     print(fsm.state)
    elif (ctrl=="5"):
        fsm.resp_ready()
        print(fsm.state)
    elif (ctrl=="6"):
        fsm.new_order()
        print(fsm.state)
    elif (ctrl=="7"):
        fsm.need_data()
        print(fsm.state)
    elif (ctrl=="8"):
        fsm.no_need_data()
        print(fsm.state)
    elif (ctrl=="9"):
        fsm.data_ready()
        print(fsm.state)
        
# fsm.start()
#fsm.machine.get_graph().draw('control_fsm.png',prog='dot')