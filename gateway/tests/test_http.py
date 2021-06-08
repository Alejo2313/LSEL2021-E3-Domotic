
import os
import sys
import inspect
import unittest
import json

# For selecting the correct path
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir) + "/fsm" 
sys.path.insert(0, parentdir)

from http_client import http_client


class Test_control_flow_http(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        """generates http parameters before each test is launched
        """
        cls._httpc =  http_client("server","40.114.216.24")
        cls._header =  {"Content-type": "application/json",
            "Accept" : "text/plain"}
    
    def test_void_name(self):
        """checks the non void name parameter
        """
        self.assertRaises(ValueError,http_client,"","40.114.216.24")

    def test_void_server_ip(self):
        """checks the non void name parameter
        """
        self.assertRaises(ValueError,http_client,"server","")
    
    def test_void_get_path(self):
        """checks the non void path parameter in do_get function
        """
        self.assertRaises(ValueError,self._httpc.do_get,"")

    def test_void_post_path(self):
        """checks the non void path parameter in do_post function
        """
        content =  {
            "name" : "Hello",
            "value": "World",
            "back" : "0"
        }
        self.assertRaises(ValueError,self._httpc.do_post,"",json.dumps(content),
        self._header)

    def test_void_post_params(self):
        """checks the non void params parameter in do_post function
        """
        self.assertRaises(ValueError,self._httpc.do_post,"/hello",json.dumps({}),
        self._header)

    def test_void_post_headers(self):
        """checks the non void headers parameter in do_post function
        """
        content =  {
            "name" : "Hello",
            "value": "World",
            "back" : "0"
        }
        self.assertRaises(ValueError,self._httpc.do_post,"/hello",content,
        json.dumps({}))