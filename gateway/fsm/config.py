
class config_read (object):
    
    def __init__(self, path, f_name):
        if (path == "" or f_name == ""):
            raise ValueError("The name of the file or path must be non empty")
        else:
            self.path = path
            self.f_name = f_name
    

    def get_path(self):
        return self.path

    def get_f_name(self):
        return self.f_name

    def exist(self, path, file):
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
                print(loc)
            else:
                raise ValueError("File to be read is not .ini file")

            try:
                f = open(loc)
            except IOError:
                print("File doesn't exist in specified path: " + loc)
                return False
            finally:
                f.close()
                return True


    

test_exist = config_read("/config/", "config.ini")
test_exist.exist("config//", "config.ini")
