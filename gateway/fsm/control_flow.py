from transitions.extensions import GraphMachine as Machine

class Control_flow (object):
    states = ['CONFIG','IDLE','PROCESS_DATA','PROCESS_ORDER']
    transitions=[
        {'trigger' : 'start', 'source' : 'CONFIG',
            'dest' : 'IDLE', 'conditions' :['ready_to_work']},
        {'trigger' : 'new_data', 'source' : 'IDLE',
            'dest' : 'PROCESS_DATA', 'after':'send_data'},        
        {'trigger' : 'need_response', 'source' : 'PROCESS_DATA',
            'dest' : '=', 'after':'wait'},        
        {'trigger' : 'no_need_command', 'source' : 'PROCESS_DATA',
            'dest' : 'IDLE'},        
        {'trigger' : 'resp_ready', 'source' : 'PROCESS_DATA',
            'dest' : 'IDLE', 'after':'send_order'},             
        {'trigger' : 'new_order', 'source' : 'IDLE',
            'dest' : 'PROCESS_ORDER', 'after':'send_order'},                
        {'trigger' : 'need_data', 'source' : 'PROCESS_ORDER',
            'dest' : '=', 'after':'wait'},                 
        {'trigger' : 'no_need_data', 'source' : 'PROCESS_ORDER',
            'dest' : 'IDLE'},                 
        {'trigger' : 'data_ready', 'source' : 'PROCESS_ORDER',
            'dest' : 'IDLE', 'after':'send_data'}
    ]
    def __init__(self,name):
        if name == "":
            raise ValueError("The name of the control flow must be non empty")
        self.name = name
        self.machine = Machine(model = self, states = Control_flow.states,
            initial = 'CONFIG', transitions=Control_flow.transitions)

    def send_data(self):
        print("data sent")

    def wait(self):
        print("waiting")

    def send_order(self):
        print("order sent")

    def ready_to_work(self):
        return True



fsm = Control_flow("GW")
# print(fsm.state)
# while (True):
#     ctrl=input("Insert key:")
#     if (ctrl=="1"):
#         fsm.start()
#         print(fsm.state)
#     elif (ctrl=="2"):
#         fsm.new_data()
#         print(fsm.state)
#     elif (ctrl=="3"):
#         fsm.need_response()
#         print(fsm.state)
#     elif (ctrl=="4"):
#         fsm.no_need_command()
#         print(fsm.state)
#     elif (ctrl=="5"):
#         fsm.resp_ready()
#         print(fsm.state)
#     elif (ctrl=="6"):
#         fsm.new_order()
#         print(fsm.state)
#     elif (ctrl=="7"):
#         fsm.need_data()
#         print(fsm.state)
#     elif (ctrl=="8"):
#         fsm.no_need_data()
#         print(fsm.state)
#     elif (ctrl=="9"):
#         fsm.data_ready()
#         print(fsm.state)
        
# fsm.start()
fsm.machine.get_graph().draw('control_fsm.png',prog='dot')