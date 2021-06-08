import os
import sys
import inspect
import unittest
import json

# For selecting the correct path
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir) + "/fsm" 
sys.path.insert(0, parentdir)

from config import config_read


class Test_config_read(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """generates a fsm object before each test is launched
        """
        cls._config =   config_read("config","config.ini")
        cls._config.read_config("../fsm/config", cls._config.get_f_name())
    
    def test_void_path(self):
        """Checks the non void path parameter
        """
        self.assertRaises(ValueError,config_read,"","config.ini")

    def test_void_file_name(self):
        """Checks the non void f_name parameter
        """
        self.assertRaises(ValueError,config_read,"config","")

    def test_default_server_ip(self):
        """Checks that default server ip is localhost
        """
        self.assertEqual("40.114.216.24",self._config.get_server_ip())

    def test_default_header_json(self):
        """Checks that default header json is value
        """
        value = json.dumps({"Content-type":"application/json","Accept":"text/plain"})
        self.assertEqual(value.replace(" ",""),self._config.get_header_json())

    def test_default_broker_ip(self):
        """Checks that default broker ip is 51.103.29.76
        """
        value = "51.103.29.76"
        self.assertEqual(value,self._config.get_broker_ip())

    def test_default_root_topic(self):
        """Checks that default root topic is /HOME
        """
        value = "/HOME"
        self.assertEqual(value,self._config.get_root_topic())

    def test_default_allowed_user(self):
        """Checks that default allowed user is POC
        """
        value = "POC"
        self.assertEqual(value,self._config.get_gateway_user())
    