import http.client
import json

class http_client(object):
    def __init__(self, name, server_ip):
        if (name == "" or server_ip == ""):
            raise ValueError("The name of name and server_ip must be non empty")
        self.name = name
        self.server_ip = server_ip
    
    def get_name(self):
        return self.name
    
    def get_server_ip(self):
        return self.server_ip
    
    def do_get(self,path):
        if (path == ""):
            raise ValueError("Path value in do_get function must be non empty")
        conn = self.create_connection()
        conn.request("GET",path)
        r1 = conn.getresponse()
        return r1.status
    
    def do_post(self,path,params, headers):
        if (path == ""):
            raise ValueError("Path value in do_post function must be non empty")
        elif (params == json.dumps({}) or headers == json.dumps({})):
            raise ValueError("params and headers value in do_post function must be non empty")
        
        conn = self.create_connection()
        conn.request("POST",path,params, headers)
        response = conn.getresponse()
        return response.status

    def create_connection(self):
        conn = http.client.HTTPConnection(self.get_server_ip(),80, timeout=10)
        return conn
    