import tornado.ioloop
import tornado.web
import json
import os
import mysql
import json
import numpy as np
import plotly.express as px
import plotly
import pandas as pd
import time
from datetime import datetime

from SQLHandler import QueryHandler


class BaseHandler(tornado.web.RequestHandler):

    def get_current_user(self):
        self.set_current_session()
        return self.get_secure_cookie("user")

    def get_current_id(self):
        self.set_current_session()
        return self.get_secure_cookie("id")

    def set_current_user(self, user, userId):
        self.set_current_session()
        print('setting secure cookie', user)
        self.set_secure_cookie("user", user)
        self.set_secure_cookie("id", userId)   

    def set_current_session(self):
        session = self.get_secure_cookie("session")
        if(session is None):
            self.set_secure_cookie("session", str(time.time()))
            

        
class LoginHandler(BaseHandler):

    def get(self):
        user = self.get_current_user()
        self.render('data/Login.html', user=user)

    def post(self):
        
        nickName = self.get_argument("user")
        password = self.get_argument("password")

        user = db.get_user(NickName=nickName)

        if( len(user) > 0 ):
            if(user["Password"] == password):
                self.set_current_user(nickName, str(user["id"]))

        self.redirect("/")

class RegisterHandler(BaseHandler):
    
    def get(self):
        user = self.get_current_user()
        self.render('data/Register.html', user=user)

    def post(self):
        user = self.get_argument("user")

        nickName = self.get_argument("user")
        password = self.get_argument("password")

        db.add_user(user, password, 0)
        self.redirect("/")


class IndexHandler(BaseHandler):

    def get(self):
        user = self.get_current_user()
        print('getting current user in IndexHandler', self.get_current_user()) 

        self.render('data/index.html', user=user)              

class LogoutHandler(BaseHandler):

    @tornado.web.authenticated
    def get(self):
        self.clear_cookie("user")
        self.redirect("/")

class PostDataHandler(BaseHandler):
    def check_xsrf_cookie(self): 
        pass

    def get(self):
        data = json.loads(self.request.body.decode('utf-8'))
        if ( data is None ):
            self.redirect("/")

        gateway = db.get_gateway(data["Gateway"])

        if( len(gateway) > 0):
            request = db.request_data(data["Gateway"], 0)
            self.write(json.dumps(request)) 
            print(request)

    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
      #  print(self.request.body.decode('utf-8'))
        if ( data is None ):
            self.redirect("/")

        gateway = db.get_gateway(data["Gateway"])

        if( len(gateway) > 0):
            
            for dev in data["Devices"]:
                device = db.get_device(DeviceUUID = dev["Device"] , GatewayUUID = data["Gateway"])
                if( len(device) == 0):
                    device = db.add_device(UUID = dev["Device"], GatewayUUID = data["Gateway"] )

                for sen in dev["Sensors"]:
                
                    sensor = db.get_sensor(typeS = sen["Type"] , DeviceID = device["DeviceID"])

                    if(len(sensor) == 0):
                        sensor = db.add_sensor(device["DeviceID"], sen["Type"], sen["DType"])
                    print(sensor)
                    db.push_data(GatewayUUID = data["Gateway"], DeviceUUID = dev["Device"], typeS=sen["Type"], Data = sen["data"])
        else:
            print("Gateway no found!")


class GateWayHandler(BaseHandler):

    def get(self):
        user = self.get_current_user()
        userID   = self.get_current_id()

        if (user is None):
            self.render('data/index.html', user = user)

        gateIDs = db.get_user_gateways( userID = userID)
        
        gates = []

        for  gID in gateIDs:
            gate = db.get_gateway(GatewayID = gID)
            if (len(gate) == 0 ):
                return
                
            userx = db.get_user(UserID = gate["AdminID"])
            gate["AdminNick"] = userx["NikName"]
            
            gates.append(gate)

        print(gates)
        self.render('data/Gateways.html', user = user, Gateways = gates)

    def post(self):
        user = self.get_current_user()
        userID   = self.get_current_id()

        gateUUID = self.get_argument("UUID")


        db.add_gateway(userID = userID, UUID = gateUUID )
        self.redirect("/gw")

def make_app():   
    settings = {
            "cookie_secret":"61oETzKXQAGaYdkL5gEmGeJJFuYh7EQnp2XdTP1o/Vo=",
            "login_url":"/login",
            "debug":False,
            'xsrf_cookies': True,
            }
    return tornado.web.Application([
            (r"/", IndexHandler),
            (r"/login", LoginHandler),
            (r"/logout", LogoutHandler),
            (r"/register", RegisterHandler),
            (r"/gw", GateWayHandler), 
            (r"/data", PostDataHandler)           
            ], **settings)


if __name__ == "__main__":

    
    db = QueryHandler("server", "server12345678","server_db")
    

    application = make_app()
    application.listen(8005)
    print('sample_app server started')
    tornado.ioloop.IOLoop.instance().start()