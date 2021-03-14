import mysql.connector as mysql


db = mysql.connect(
    host = "localhost",
    user = "username",
    password = "password",
    database = "LSELdata"
)

# GRANT ALL PRIVILEGES ON *.* TO 'username'@'localhost' IDENTIFIED BY 'password'; being in sudo mysql

print(db) 

mycursor = db.cursor()

mycursor.execute("CREATE TABLE IF NOT EXISTS `UserGateways`(    \
                  `UserID` int  NOT NULL ,        \
                  `GatewayID` int  NOT NULL); ")

mycursor.execute("CREATE TABLE IF NOT EXISTS `Users`(             \
                  `UserID` int  NOT NULL ,           \
                  `NickName` TEXT NOT NULL ,       \
                  `Password` varchar(64)  NOT NULL , \
                  `IsAdmin` bool  NOT NULL ,         \
                  PRIMARY KEY (`UserID`));")

mycursor.execute( "CREATE TABLE IF NOT EXISTS `Gateways` (         \
                 `GatewayID` int  NOT NULL ,         \
                 `AdminID` int  NOT NULL ,           \
                 `GatewayKey` varchar(32)  NOT NULL ,\
                 `IsValid` bool  NOT NULL ,          \
                  PRIMARY KEY (GatewayID));")    

mycursor.execute( "CREATE TABLE IF NOT EXISTS `Devices` (   \
                  `DevicesID` int  NOT NULL , \
                  `GatewayID` int  NOT NULL , \
                  `Type` int  NOT NULL ,      \
                  `DataType` int  NOT NULL ,  \
                    PRIMARY KEY (             \
                        `DevicesID` ));")


mycursor.execute( "CREATE TABLE IF NOT EXISTS `SensorData` (   \
                  `DevicesID` int  NOT NULL ,    \
                  `TimeStamp` date  NOT NULL ,   \
                  `Data` TEXT  NOT NULL);")

mycursor.execute( "CREATE TABLE IF NOT EXISTS `DeviceOrder` (  \
                  `DeviceID` int  NOT NULL ,     \
                  `TimeStamp` date  NOT NULL ,   \
                  `Data` TEXT  NOT NULL);")

# mycursor.execute( "ALTER TABLE UserGateways ADD CONSTRAINT fk_Users_UserID FOREIGN KEY(UserID) \
#                    REFERENCES Users (UserID);")

# mycursor.execute( "ALTER TABLE `Gateways` ADD CONSTRAINT `fk_Gateways_AdminID` FOREIGN KEY(`AdminID`) \
#                     REFERENCES `Users` (`UserID`);")

# mycursor.execute( "ALTER TABLE `UserGateways` ADD CONSTRAINT `fk_UserGateways_GatewayID` FOREIGN KEY(`GatewayID`) \
#                    REFERENCES `Gateways` (`GatewayID`);")

# mycursor.execute( "ALTER TABLE `DeviceOrder` ADD CONSTRAINT `fk_Devices_DevicesID` FOREIGN KEY(`DeviceID`) \
#                    REFERENCES `Devices` (`DevicesID`);")

# mycursor.execute( "ALTER TABLE `Devices` ADD CONSTRAINT `fk_Devices_GatewayID` FOREIGN KEY(`GatewayID`) \
#                    REFERENCES `Gateways` (`GatewayID`);")

# mycursor.execute( "ALTER TABLE `SensorData` ADD CONSTRAINT `fk_SensorData_DevicesID` FOREIGN KEY(`DevicesID`) \
#                    REFERENCES `Devices` (`DevicesID`);")