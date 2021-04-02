import http.client
import json

class http_client(object):
    def __init__(self, name, server_ip):
        self.name = name
        self.server_ip = server_ip
    
    def get_name(self):
        return self.name
    
    def get_server_ip(self):
        return self.server_ip
    
    def do_get(self,path):
        conn = self.create_connection()
        conn.request("GET",path)
        r1 = conn.getresponse()
        return r1.status
    
    def do_post(self,path,params, headers):
        conn = self.create_connection()
        conn.request("POST",path,params, headers)
        response = conn.getresponse()
        return response.status

    def create_connection(self):
        conn = http.client.HTTPConnection(self.server_ip(),80, timeout=10)
        return conn
    

        
#conn = http.client.HTTPConnection("40.114.216.24",80, timeout=10)
# conn.request("GET", "/")
# r1 = conn.getresponse()
# print(r1.status, r1.reason)
# data1 = r1.read()
# print(data1)

# params = {
#     "name" : "Hello",
#     "value": "World"
# }

# headers = {"Content-type": "application/json",
#     "Accept" : "text/plain"}

# conn.request("POST","/gateData",json.dumps(params),headers)
# response = conn.getresponse()
# print(response.status,response.reason)
# conn.close()