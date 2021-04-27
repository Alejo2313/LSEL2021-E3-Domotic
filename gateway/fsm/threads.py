from transitions.extensions import LockedMachine as Machine
from threading import Thread
import sys
import time



states_A = ['A', 'B', 'C']
machine = Machine(states=states_A, initial='A')

states_B = ['M','N','O']
machineB = Machine(states = states_B, initial = 'M')
def func_states_A():
    while(1):
        ctrl = input("Insert a letter for fsm A: \n")
        if (ctrl == '1'):
            machine.to_A()
        elif (ctrl == '2'):
            machine.to_B()
        elif (ctrl == '3'):
            machine.to_C()
        print(machine.state)

def func_states_B():
    while(1):
        ctrl = input("Insert a letter for fsm B: \n")
        if (ctrl == 'a'):
            machineB.to_M()
        elif (ctrl == 's'):
            machineB.to_M()
        elif (ctrl == 'd'):
            machineB.to_O()
        print(machineB.state)

thread_A = Thread(target=func_states_A)
thread_A.start()

thread_B = Thread(target = func_states_B)
thread_B.start()
thread_A.join()