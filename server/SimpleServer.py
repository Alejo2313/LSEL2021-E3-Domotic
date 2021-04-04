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
from datetime import datetime
from SQLHandler import QueryHandler


class BaseHandler(tornado.web.RequestHandler):

    def get_current_user(self):
        return self.get_secure_cookie("user")

    def get_current_id(self):
        return self.get_secure_cookie("id")

    def set_current_user(self, user, userId):
        print('setting secure cookie', user)
        self.set_secure_cookie("user", user)
        self.set_secure_cookie("id", userId)   

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
            ], **settings)


if __name__ == "__main__":

    
    db = QueryHandler("server", "server12345678","server_db")
    

    print(db.get_gateway("ABDCEH"))
    print(db.add_device(UUID=2, GatewayUUID="ABDCEH"))

    device = db.get_device(DeviceUUID = 2, GatewayUUID ="ABDCEH")

    print(db.add_sensor(device["DeviceID"], 1, 1))

    db.push_data(GatewayUUID="ABDCEH", DeviceUUID= device["UUID"], typeS = 1, Data = 33  )

    db.push_request_data(GatewayUUID="ABDCEH", DeviceUUID= device["UUID"], typeS = 1, Data = 33  )
    db.push_request_data(GatewayUUID="ABDCEH", DeviceUUID= device["UUID"], typeS = 1, Data = 34  )
    db.push_request_data(GatewayUUID="ABDCEH", DeviceUUID= device["UUID"], typeS = 1, Data = 35  )


    print( db.request_data( GatewayUUID = "ABDCEH", updated = 0))

    application = make_app()
    application.listen(8005)
    print('sample_app server started')
    tornado.ioloop.IOLoop.instance().start()