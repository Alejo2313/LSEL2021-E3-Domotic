import os
import sys
import inspect
import unittest

# For selecting the correct path
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir) + "/fsm" 
sys.path.insert(0, parentdir)

from control_flow import Control_flow

class Test_control_flow_transitions(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """generates a fsm object before each test is launched
        """
        cls._fsm =  Control_flow("GW")

    def test_void_name(self):
        """checks the non void name parameter
        """
        self.assertRaises(ValueError,Control_flow,"")

    def test_initial_state(self):
        """checks that the initial state is CONFIG
        """
        self._fsm.to_CONFIG()
        self.assertEqual(self._fsm.state,"CONFIG")
    
    def test_to_idle(self):
        self._fsm.to_CONFIG()
        self._fsm.start()
        self.assertEqual(self._fsm.state,"IDLE")

    def test_new_data(self):
        self._fsm.to_IDLE()
        self._fsm.new_data()
        self.assertEqual(self._fsm.state,"PROCESS_DATA")
    
    def test_needResponse(self):
        self._fsm.to_PROCESS_DATA()
        self._fsm.need_response()
        self.assertEqual(self._fsm.state,"PROCESS_DATA")

    def test_no_need_response(self):
        self._fsm.to_PROCESS_DATA()
        self._fsm.no_need_response()
        self.assertEqual(self._fsm.state,"IDLE")

    def test_resp_ready(self):
        self._fsm.to_PROCESS_DATA()
        self._fsm.resp_ready()
        self.assertEqual(self._fsm.state, "IDLE")

    def test_new_order(self):
        self._fsm.to_IDLE()
        self._fsm.new_order()
        self.assertEqual(self._fsm.state, "PROCESS_ORDER")

    def test_needData(self):
        self._fsm.to_PROCESS_ORDER()
        self._fsm.need_data()
        self.assertEqual(self._fsm.state, "PROCESS_ORDER")

    def test_no_need_data(self):
        self._fsm.to_PROCESS_ORDER()
        self._fsm.no_need_data()
        self.assertEqual(self._fsm.state,"IDLE")

    def test_data_ready(self):
        self._fsm.to_PROCESS_ORDER()
        self._fsm.data_ready()
        self.assertEqual(self._fsm.state,"IDLE")