from transitions.extensions import GraphMachine as Machine


class Controll_flow (object):
    states = ['CONFIG','IDLE','PROCESS_DATA','PROCESS_ORDER']
    def __init__(self,name):
        if name == "":
            raise ValueError("The name of the control flow must be non empty")
        self.name = name

        self.machine = Machine(model = self, states = Controll_flow.states,
            initial = 'CONFIG')

        self.machine.add_transition(trigger = 'start', source = 'CONFIG',
            dest = 'IDLE', conditions =['ready_to_work'])
        self.machine.add_transition(trigger = 'newData', source = 'IDLE',
            dest = 'PROCESS_DATA')
        self.machine.add_transition('needResponse','PROCESS_DATA', '=')
        self.machine.add_transition('noNeedResponse','PROCESS_DATA', 'IDLE')
        self.machine.add_transition('respReady','PROCESS_DATA','IDLE')
        self.machine.add_transition('newOrder','IDLE','PROCESS_ORDER')
        self.machine.add_transition('needData','PROCESS_ORDER','=')
        self.machine.add_transition('noNeedData','PROCESS_ORDER', 'IDLE')
        self.machine.add_transition('dataReady','PROCESS_ORDER','IDLE')

    def ready_to_work(self):
        return True
fsm = Controll_flow("GW")
fsm.machine.get_graph().draw('control_fsm.png',prog='dot')