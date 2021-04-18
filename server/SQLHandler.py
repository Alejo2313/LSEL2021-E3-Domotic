
import mysql.connector
from datetime import datetime



class QueryHandler:

    def __init__ (self, user, password, database, host = '127.0.0.1'):
        self.cnx = mysql.connector.connect(
                                user=user, 
                                password=password,
                                host=host,
                                database=database,
                                use_pure=False)
        self.cursor = self.cnx.cursor()

    def add_user(self, NickName, Password, IsAdmin):

        query = """ INSERT INTO Users (NickName, Password, IsAdmin)
                VALUES (%s, %s, %s)"""

        if len(self.get_user(NickName = NickName)) > 0:
            return {}

        data  = (NickName, Password, IsAdmin)
        self.cursor.execute(query, data)
        self.cnx.commit()

        return self.get_user(NickName = NickName)

    def get_user(self, UserID = -1, NickName = None,):

        if( NickName is not None): 
            query = "SELECT * FROM Users WHERE NickName = %s"
            data  = (NickName,)
        else:
            query = "SELECT * FROM Users WHERE UserID = %s"
            data  = (UserID,)

        self.cursor.execute(query, data)

        raw = self.cursor.fetchone()

        if raw is not None:
            user = {
                "id": raw[0],
                "NikName": raw[1],
                "Password": raw[2],
                "IsAdmin": raw[3]
            }
        else:
            user = {}
        
        return user

    def add_gateway(self, userID, UUID, NickName = None):
        query = """ INSERT INTO Gateways (AdminID, GatewayKey, IsValid)
            VALUES (%s, %s, %s)"""

        if (NickName is not None):
            user = self.get_user(NickName= NickName)
            if len(user) == 0:
                return {}
            userID = user["id"]
        
        data = (userID, UUID, 1)

        if len(self.get_gateway(UUID)) > 0:
            return {}
        
        self.cursor.execute(query, data)
        self.cnx.commit()

        gateway = self.get_gateway(UUID)

        self.add_user_gateways(gateway["AdminID"], gateway["GatewayID"], gateway["AdminID"])
        return gateway

    def get_gateway(self, UUID = -1, GatewayID = None):

        if (GatewayID is None):
            query = "SELECT * FROM Gateways WHERE GatewayKey = %s"
            data  = (UUID,)
        else:
            query = "SELECT * FROM Gateways WHERE GatewayID  = %s"
            data  = (GatewayID,)

        self.cursor.execute(query, data)

        raw = self.cursor.fetchone()

        if raw is not None:
            gateway = {
                "GatewayID": raw[0],
                "AdminID": raw[1],
                "UUID": raw[2],
                "IsValid": raw[3]
            }

        else:
            gateway = {}
        
        return gateway       

    def get_user_gateways(self, userID = -1, NickName = None):

        if NickName is not None:
            user = self.get_user(NickName=NickName)
            if len(user) == 0:
                return []
            userID = user["id"]

        query = "SELECT GatewayID FROM UserGateways WHERE UserID = %s"
        data  = (userID,)

        self.cursor.execute(query, data)
        result = []
        for (GatewayID,) in self.cursor.fetchall():
            result.append(GatewayID)
        
        return result

    def get_auth_users(self, userID):
    
        query = """ SELECT Users.NickName, Users.UserID, Gateways.GatewayKey  
                    FROM Users
                    INNER JOIN UserGateways ON Users.UserID = UserGateways.UserID
                    INNER JOIN Gateways ON Gateways.GatewayID = UserGateways.GatewayID

                    WHERE Gateways.AdminID = %s AND Users.UserID != Gateways.AdminID
               """
        data = (userID, )

        self.cursor.execute(query, data)

        raw = self.cursor.fetchall()

        users = []
        for (NickName, UserID, UUID) in raw:
            user = {
                "NickName": NickName,
                "UserID": UserID,
                "UUID": UUID
            }
            users.append(user)
        return users

    def add_user_gateways(self, userID, GatewayID, AdminID):

        query = "INSERT INTO UserGateways (UserID, GatewayID) VALUES (%s, %s)"
        data = (userID, GatewayID)

        gateway = self.get_gateway("", GatewayID = GatewayID)
        if len(gateway) > 0 and gateway["AdminID"] == int(AdminID):
            self.cursor.execute(query, data)
            self.cnx.commit()

            return self.get_user_gateways(userID)
        else:
            return []

    def get_device(self, GatewayID = -1, DeviceID = -1, DeviceUUID = None, GatewayUUID = None):
    
        if (GatewayUUID is not None ):
            gateway = self.get_gateway(GatewayUUID)
            if len(gateway) == 0:
                return {}
            GatewayID = gateway["GatewayID"]

        if DeviceUUID is not None:
            query = "SELECT * FROM Devices WHERE GatewayID = %s AND UUID = %s"
            data = (GatewayID, DeviceUUID)
        else:
            query = "SELECT * FROM Devices WHERE DeviceID = %s"
            data = (DeviceID,)

        self.cursor.execute(query, data)
        raw = self.cursor.fetchone()

        if raw is not None:
            device = {
                "DeviceID": raw[0],
                "GatewayID": raw[1],
                "UUID":raw[2]
            }

            return device
        else:
            return {}

    def get_gw_devices(self, GatewayID):
        query = "SELECT * FROM Devices WHERE GatewayID = %s"
        data = (GatewayID,)
        self.cursor.execute(query, data)
        raw = self.cursor.fetchall()

        devices = []
        for (DeviceID, GatewayID, UUID) in raw:
            dv = {
                "DeviceID": DeviceID,
                "GatewayID": GatewayID,
                "UUID":UUID
            }
            devices.append(dv)

        return devices

    def get_dv_sensors(self, deviceID):
        query = "SELECT * FROM Sensors WHERE DeviceID = %s"
        data = (deviceID,)
        self.cursor.execute(query, data)
        raw = self.cursor.fetchall()
        sensors = []
        for (DeviceID, SensorID, Type, DataType) in raw:
            sensor = {
                "DeviceID": DeviceID,
                "SensorID": SensorID,
                "Type":     Type,
                "DataType": DataType
            }
            sensors.append(sensor)
        return sensors
        º
 
    def add_device(self, GatewayID = -1, UUID = None, GatewayUUID = None):
        if GatewayUUID is not None:
            gateway = self.get_gateway(GatewayUUID)
            if(len(gateway) == 0):
                return {}
            GatewayID = gateway["GatewayID"]

        query = "INSERT INTO Devices (GatewayID, UUID) VALUES (%s, %s)"
        data = (GatewayID, UUID)
 
        if (len(self.get_device(GatewayID=GatewayID, DeviceUUID=UUID )) > 0  ):
            return {}

        self.cursor.execute(query, data)
        self.cnx.commit()

        return self.get_device(GatewayID=GatewayID, DeviceUUID=UUID)

    def get_sensor(self, SensorID = -1, typeS = None, DeviceID = None):

        if(DeviceID is not None):
            query = "SELECT * FROM Sensors WHERE DeviceID = %s AND Type = %s"
            data = (DeviceID, typeS)
        else:
            query = "SELECT * FROM Sensors WHERE SensorID = %s"
            data = (SensorID,)

        self.cursor.execute(query, data)
        raw = self.cursor.fetchone()

        if raw is not None:
            sensor = {
                "DeviceID": raw[0],
                "SensorID": raw[1],
                "Type":     raw[2],
                "DataType": raw[3]
            }

            return sensor
        else:
            return {}

    def add_sensor(self, DeviceID, typeS, DataType):
        query = "INSERT INTO Sensors (DeviceID, Type, DataType) VALUES (%s, %s, %s)"
        data  = (DeviceID, typeS, DataType)
        

        sensor = self.get_sensor(typeS=typeS, DeviceID=DeviceID)

        if ( len(sensor) > 0):
            return {}
        

        self.cursor.execute(query, data)
        self.cnx.commit()

        return self.get_sensor(typeS=typeS, DeviceID=DeviceID)

    def push_data(self, GatewayUUID, DeviceUUID, typeS, Data):

        device = self.get_device(GatewayUUID = GatewayUUID, DeviceUUID= DeviceUUID)

        if(len(device) == 0):
            return

        sensor = self.get_sensor(typeS= typeS, DeviceID=device["DeviceID"])

        if ( len(sensor) == 0):
            return

        now = datetime.now()
        query = "INSERT INTO InData (SensorID, TimeStamp, Data) VALUES (%s, %s, %s)"
        data  = (sensor["SensorID"],now, Data )

        self.cursor.execute(query, data)
        self.cnx.commit()
    
    def request_data(self, GatewayUUID, updated):

        query = """SELECT Devices.UUID, Sensors.Type, OutData.* 
                FROM (((OutData 
                INNER JOIN Sensors  ON Sensors.SensorID = OutData.SensorID)
                INNER JOIN Devices  ON Devices.DeviceID = Sensors.DeviceID)
                INNER JOIN Gateways ON Gateways.GatewayID = Devices.GatewayID)

                WHERE Gateways.GatewayKey = %s  AND OutData.updated = %s """
    
        data = (GatewayUUID,updated )

        self.cursor.execute(query, data)

        raw = self.cursor.fetchall()

        outData = []
        for (UUID, SensorType, SensorID, TimeStamp, Data, updated ) in raw:
            element = {
                "Device": UUID,
                "SensorType": SensorType,
                "SensorID":SensorID,
                "TimeStamp":str(TimeStamp),
                "Data":Data,
                "updated":updated
            }
            outData.append(element)
        

        query = """UPDATE OutData 
                
                INNER JOIN Sensors  ON Sensors.SensorID = OutData.SensorID
                INNER JOIN Devices  ON Devices.DeviceID = Sensors.DeviceID
                INNER JOIN Gateways ON Gateways.GatewayID = Devices.GatewayID

                SET  OutData.updated = 1

                WHERE Gateways.GatewayKey = %s  AND OutData.updated = %s """ 

        self.cursor.execute(query, data)
        self.cnx.commit()

        return outData
    
    def push_request_data(self, SensorID = None, Data = 0, GatewayUUID = None , DeviceUUID = None, typeS = None):

        now = datetime.now()

        if SensorID is None:
            device = self.get_device(GatewayUUID = GatewayUUID, DeviceUUID= DeviceUUID)

            if(len(device) == 0):
                return

            sensor = self.get_sensor(typeS= typeS, DeviceID=device["DeviceID"])

            if ( len(sensor) == 0):
                return
            
            SensorID = sensor["SensorID"]

       
        query = "INSERT INTO OutData (SensorID, TimeStamp, Data, updated) VALUES (%s, %s, %s,%s)"
        data  = (SensorID ,now, Data, 0 )

        self.cursor.execute(query, data)
        self.cnx.commit()
    
    def get_data(self, sensorID, fromDate = None, toDate = None, limit= None):
    
        if limit is not None:
            query = "SELECT * FROM InData WHERE SensorID = %s LIMIT %s"
            data = (sensorID, limit)
        else:
            query = "SELECT * FROM InData WHERE SensorID = %s AND TimeStamp BETWEEN %s AND %s"
            data = (sensorID, fromDate, toDate)

        self.cursor.execute(query, data)

        raw = self.cursor.fetchall()

        outData = []
        for (SensorID, TimeStamp, Data ) in raw:
            element = {
                "SensorID":SensorID,
                "TimeStamp":TimeStamp,
                "Data":Data,
            }
            outData.append(element)
        
        return outData