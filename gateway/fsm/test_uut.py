import unittest
from control_flow import Controll_flow

class Test_control_flow_transitions(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """generates a fsm object before each test is launched
        """
        cls._fsm =  Controll_flow("GW")

    def test_void_name(self):
        """checks the non void name parameter
        """
        self.assertRaises(ValueError,Controll_flow,"")

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
        self._fsm.newData()
        self.assertEqual(self._fsm.state,"PROCESS_DATA")
    
    def test_needResponse(self):
        self._fsm.to_PROCESS_DATA()
        self._fsm.needResponse()
        self.assertEqual(self._fsm.state,"PROCESS_DATA")

    def test_no_need_response(self):
        self._fsm.to_PROCESS_DATA()
        self._fsm.noNeedResponse()
        self.assertEqual(self._fsm.state,"IDLE")

    def test_resp_ready(self):
        self._fsm.to_PROCESS_DATA()
        self._fsm.respReady()
        self.assertEqual(self._fsm.state, "IDLE")

    def test_new_order(self):
        self._fsm.to_IDLE()
        self._fsm.newOrder()
        self.assertEqual(self._fsm.state, "PROCESS_ORDER")

    def test_needData(self):
        self._fsm.to_PROCESS_ORDER()
        self._fsm.needData()
        self.assertEqual(self._fsm.state, "PROCESS_ORDER")

    def test_no_need_data(self):
        self._fsm.to_PROCESS_ORDER()
        self._fsm.noNeedData()
        self.assertEqual(self._fsm.state,"IDLE")

    def test_data_ready(self):
        self._fsm.to_PROCESS_ORDER()
        self._fsm.dataReady()
        self.assertEqual(self._fsm.state,"IDLE")