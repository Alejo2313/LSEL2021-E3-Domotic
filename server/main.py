import tornado.web
import tornado.ioloop
import mysql.connector 
import json


class indexHandler(tornado.web.RequestHandler):
    def post(self):
        print("do post")
        

    def get(self):
        print("do get")
        self.write("Hello, world")

class gatewayData(tornado.web.RequestHandler):
    def post(self):
        data = json.loads(self.request.body.decode('utf-8'))
        print('Got JSON data:',type( self.request.body.decode('utf-8') ) )
        

if (__name__ == "__main__"):
    app = tornado.web.Application([
        ("/", indexHandler),
        ("/gateData", gatewayData)
    ])

    app.listen(80, max_body_size=200 * 1024 * 1024)
    print("Listening on port 80")
    tornado.ioloop.IOLoop.instance().start()