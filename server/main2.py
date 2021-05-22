import tornado.web
import tornado.ioloop
import os
import mysql
import json
import matplotlib.pyplot as plt
from matplotlib.backends.backend_webagg_core import (
    FigureManagerWebAgg, new_figure_manager_given_figure)
from matplotlib.figure import Figure
import numpy as np
import base64
from io import BytesIO
import codecs

########### mySQL
# config = {
#     "user":     "",
#     "password": "",
#     "host":     "localhost",
#     "database": "test",
#     "use_pure": True,
# }

# b_data = json.dumps(json_data) #python dictionary to json
# data = json.loads(json_data) #json to python dictionary
# docID = 1

##############
temp = []
t_sample = []

class saveTemp(tornado.web.RequestHandler):
    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
        temp.append(data["Temp"])
        t_sample.append(data["Time"])
        user = data["User"]
        print(data)
        print(len(temp))
        if len(temp) >= 5:
            plt.figure()
            plt.subplot(111)
            plt.plot(t_sample, temp)
            plt.xlabel('time (s)')
            plt.ylabel('temp (Celsius)')
            plt.title('Temperature')
            tmpfile = BytesIO()
            plt.savefig(tmpfile, format='png')
            encoded = base64.b64encode(tmpfile.getvalue()).decode('utf-8')
            plt.savefig("test.png")
            html = '<img src=\'data:image/png;base64,{}\'>'.format(encoded) 
            with open('test.html','w') as f:
                f.write(html)


#        print('Got JSON data:', data)

        #print('Got JSON data:',self.request.body.decode('utf-8') )


class OnOff(tornado.web.RequestHandler):
    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
        if data["Temp"] > 18:
            response = {"Heater": "off"}
        else:
            response = {"Heater": "on"}
        self.write(json.dumps(response))
            

class showTemp(tornado.web.RequestHandler):
    def get(self):
        with open('test.html','r') as f:
                self.write(f.read())

class saveData(tornado.web.RequestHandler):
    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
#        print('Got JSON data:', data)
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


if (__name__ == "__main__"):
    #initialize database
    app = tornado.web.Application([tornado.web.url("/hello",HelloWorld),tornado.web.url('/onoff', OnOff), tornado.web.url('/temp', saveTemp), tornado.web.url('/show', showTemp)])
    http_server = tornado.httpserver.HTTPServer(app)
    app.listen(80)
    print("Listening on port 80")
    tornado.ioloop.IOLoop.instance().start()
