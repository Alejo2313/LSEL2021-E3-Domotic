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
    msg_rcvd = ""
    msg_json = ""
    
    def __init__(self,name):
        if name == "":
            raise ValueError("The name of the control flow must be non empty")
        self.name = name
        self.machine = Machine(model = self, states = Control_flow.states,
            initial = 'CONFIG', transitions=Control_flow.transitions)
        self.http_con = http_client("server",Control_flow.server_ip)

    def send_data(self):
        if (Control_flow.msg_rcvd[len(Control_flow.msg_rcvd) -1] == "0"):
            fsm.no_need_response()
            self.http_con.do_post("/gateData",
            self.msg_json,self.header_json)
        else:
            fsm.need_response()

        print("data sent")
        print(fsm.state)

    def wait(self):
        print("waiting")

    def send_order(self):
        print("order sent")

    def ready_to_work(self):
        return self.http_con.do_get("/") == 200

    def msg_deliver(self):
        return self.http_con.do_post("/gateData",
            json.dumps(params),self.header_json) == 200
    


def on_message_suscriber(client, userdata, msg):
    raw_msg = str(msg.payload)
    raw_tpc = str(msg.topic)
    print("raw msg: " + raw_msg + "length = " + str(len(raw_msg)))
    clean_msg = raw_msg[2:len(raw_msg)-1]
    clean_tpc = raw_tpc.split("-")
    print(clean_msg)
    Control_flow.msg_rcvd = clean_msg
    Control_flow.msg_json = json.dumps(
        {
            "gateway_name" : "Gateway_test",
            "local_id" : clean_tpc[0],
            "data_t" : clean_tpc[1],
            "src_t" : clean_tpc[2],
            "src_id" : clean_tpc[3],
            "data" : clean_msg
        }
    )
    fsm.new_data()

fsm = Control_flow("GW")
print(fsm.state)
params = {
    "name" : "Hello",
    "value": "World",
    "back" : "0"
}
while (True):
    ctrl=input("Insert key:")
    if (ctrl=="1"):
        fsm.start()
        print(fsm.state)
        sub1=mqtt_subscriber(broker_addr=Control_flow.broker_ip, gw_name="gw",client_id="sub",
            subscribe_topic="/casa",on_msg_function = on_message_suscriber)
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
    # elif (ctrl=="5"):
    #     fsm.resp_ready()
    #     print(fsm.state)
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