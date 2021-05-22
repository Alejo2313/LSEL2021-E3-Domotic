import os
import json


class config_read (object):
    
    def __init__(self, path, f_name, server_ip = None, broker_ip = None,
        header_json = None, root_topic = None):
        if (path == "" or f_name == ""):
            raise ValueError("The name of the file or path must be non empty")
        else:
            self.path = path
            self.f_name = f_name
            self.server_ip = ""
            self.broker_ip = ""
            self.header_json = ""
            self.root_topic = ""
    

    def get_path(self):
        return self.path

    def get_f_name(self):
        return self.f_name
    
    def get_server_ip(self):
        return self.server_ip

    def get_broker_ip(self):
        return self.broker_ip
    
    def get_header_json(self):
        return self.header_json

    def get_root_topic(self):
        return self.root_topic

    def exist(self, path, file):
        def create_config(path, file):
            if (os.path.isdir("./" + path) == False):
                os.mkdir("./" + path)       
            header_json_aux = {"Content-type": "application/json", "Accept" : "text/plain"}

            config_file = open("./" + path + "/" + file, "w")
            config_file.write("#config server\n")
            config_file.write("server.ip = localhost\n")
            config_file.write("header_json = " + json.dumps(header_json_aux))
            config_file.write("\n")
            config_file.write("#config mqtt\n")
            config_file.write("broker.ip = localhost\n")
            config_file.write("root_topic = /home\n")
            config_file.close()


        if (path == "" or file == ""):
            raise ValueError("The name of the file or path must be non empty")
        else:
            
            if (path[0] == "/"):
                c_path = path
            else:
                c_path = "/" + path
            
            if (path[len(path)-1] == "/"):
                c2_path = c_path
            else:
                c2_path = c_path + "/"

            if (file.find(".ini") != -1):
                loc = "." + str(c2_path + file)
            else:
                raise ValueError("File to be read is not .ini file")

            try:
                f = open(loc)
            except IOError:
                print("Creating config.ini on specified file: " + loc)
                create_config(path, file)
                return False
            finally:
                return True

    def read_config(self, path, file):
        
        def parse_server_params(file_list, pos):
            res = ["", ""]
            for j in range(pos + 1, pos + 3):
                aux = file_list[j].replace(" ", "")
                aux = aux.split("=")
                if (aux[0] == "" or aux[1] == ""):
                    raise ValueError("Param or value must be non empty")
                elif (aux[0] == "server.ip"):
                    res[0] = aux[1]
                elif (aux[0] == "header_json"):
                    res[1] = aux[1]
                else:
                    raise ValueError("Param not valid")
            return res

        def parse_mqtt_params(file_list, pos):
            res = ["", ""]
            for j in range(pos + 1, pos + 3):
                aux = file_list[j].replace(" ", "")
                aux = aux.split("=")
                if (aux[0] == "" or aux[1] == ""):
                    raise ValueError("Param or value must be non empty")
                elif (aux[0] == "broker.ip"):
                    res[0] = aux[1]
                elif (aux[0] == "root_topic"):
                    res[1] = aux[1]
                else:
                    raise ValueError("Param not valid")
            return res
                

        if (path == "" or file == ""):
            raise ValueError("The name of the file or path must be non empty")
        else:
            if self.exist(path,file):
                with open("./" + path + "/" + file, "r") as f_config:
                    config_list = list(f_config)
                
                for i in config_list:
                    if(i == "#config server\n"):
                        server_params = parse_server_params(config_list, config_list.index(i))
                        self.server_ip = server_params[0].replace("\n","")
                        self.header_json = server_params[1].replace("\n","")
#                        print("server IP: " + (self.get_server_ip()))
#                        print("header json: " + self.get_header_json())
                    elif (i == "#config mqtt\n"):
                        mqtt_params = parse_mqtt_params(config_list, config_list.index(i))
                        self.broker_ip = mqtt_params[0].replace("\n","")
                        self.root_topic = mqtt_params[1].replace("\n","")
#                        print("Broker IP: " + self.get_broker_ip())
#                        print("Root topic: " + self.get_root_topic())

prueba = config_read("config" , "config.ini")
prueba.read_config(prueba.get_path(), prueba.get_f_name())
                        

