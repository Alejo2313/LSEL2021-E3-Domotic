import tornado.web
import tornado.ioloop
import os
import mysql
import json
import numpy as np
import mysql.connector
import plotly.express as px
import plotly
import pandas as pd
from datetime import datetime


# b_data = json.dumps(json_data) #python dictionary to json
# data = json.loads(json_data) #json to python dictionary
# docID = 1

##############
temp = [10,13,11,15]
t_sample = [10,11,10,11]
deviceID = ["T1","T1","T2","T2"]

class saveTemp(tornado.web.RequestHandler):
    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
        temp.append(data["Temp"])
        t_sample.append(data["Time"])
        deviceID.append(data["DeviceID"])
        df = pd.DataFrame(dict(temperature=temp, time=t_sample, deviceID=deviceID))
        if len(temp) >= 4:
            fig = px.line(df, x="time", y="temperature", title = "Temperature Data", color='deviceID', 
                    labels={
                     "time": "Time (h)",
                     "temperature": "Temperature (Celsius)",
                     "deviceID": "Device"})
            plotly.offline.plot(fig, filename='temp_test.html')


class OnOff(tornado.web.RequestHandler):
    def post(self):     
        self.data = json.loads(self.request.body.decode('utf-8'))
        now = datetime.now() # format: 2021-03-14 17:02:12.285803
        t1 = self.data["timestamp"]
        t2 = datetime.strptime(now, "%Y-%m-%d %H:%M:%S")
        elapsed = t2-t1 # elapsed time between data measured and time when processing
        if elapsed.seconds <= 10:
            if self.data["gateway_name"] == "LivingRoom":
                response = self.LivingRoom()
            elif self.data["gateway_name"] == "Kitchen":
                response = self.Kitchen()
            self.write(json.dumps(response))
        else 
            pass
        
        

    def LivingRoom(self): # gateway salon
        if self.data["id_local"] == 0: # for example temp and humidity sensor
            decisionFunction = {'0': self.TemperatureSensor(), '1': self.HumiditySensor()}
            response = decisionFunction[self.data["id_sensor"]]
            print(response)
        

    def TemperatureSensor(self):
        response = {"gateway_name" : self.data["gateway_name"],
                    "id_local" : self.data["id_local"],
                    "id_actuator" : self.actuatorID["heater"],
                    "type" : bool,
                    "data" : 1} # turn heater on
        
        if self.data["data"] > 20:
            response ["data"] = 0
        return response

    def HumiditySensor(self):
        # no actuator in this case, just store the data


            

class showTemp(tornado.web.RequestHandler):
    def get(self):
        with open('temp_test.html','r') as f:
                self.write(f.read())

class saveData(tornado.web.RequestHandler):
    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
        print('Got JSON data:',self.request.body.decode('utf-8') )
#       self.write({ 'got' : 'your data' })
#         db = mysql.connector.connect(**config)
#         cursor = db.cursor(prepared=True)
#         stmt = "UPDATE JSONDoc SET Data = JSON_REPLACE(Data, CONCAT('$', %s), CAST(%s AS JSON)) WHERE DocID=%s"
#         cursor.execute(stmt, ("", data, docID))
#         db.commit()
#         cursor.close()
#         db.close()


class HelloWorld(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")


if (__name__ == "__main3__"):
    #initialize database
    app = tornado.web.Application([tornado.web.url("/hello",HelloWorld),tornado.web.url('/onoff', OnOff),
        tornado.web.url('/temp', saveTemp), tornado.web.url('/show', showTemp)])
    http_server = tornado.httpserver.HTTPServer(app)
    app.listen(80)
    print("Listening on port 80")
    tornado.ioloop.IOLoop.instance().start()
