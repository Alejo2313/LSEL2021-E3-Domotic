CREATE TABLE IF NOT EXISTS `Users` (
    `UserID` int  NOT NULL AUTO_INCREMENT,
    `NickName` varchar(64) NOT NULL UNIQUE,
    `Password` varchar(64)  NOT NULL ,
    `IsAdmin` bool  NOT NULL ,
    PRIMARY KEY (
        `UserID`
    )
);

CREATE TABLE `Gateways` (
    `GatewayID` int  NOT NULL AUTO_INCREMENT,
    `AdminID` int  NOT NULL ,
    `GatewayKey` varchar(32)  NOT NULL UNIQUE,
    `IsValid` bool  NOT NULL ,
    PRIMARY KEY (
        `GatewayID`
    )
);

CREATE TABLE `UserGateways` (
    `UserID` int  NOT NULL ,
    `GatewayID` int  NOT NULL 
);

CREATE TABLE `Devices` (
    `DeviceID` int  NOT NULL AUTO_INCREMENT,
    `GatewayID` int  NOT NULL ,
    `UUID` varchar(32)  NOT NULL ,
    PRIMARY KEY (
        `DeviceID`
    )
);

CREATE TABLE `Sensors` (
    `DeviceID` int  NOT NULL ,
    `SensorID` int  NOT NULL AUTO_INCREMENT,
    `Type` int  NOT NULL ,
    `DataType` int  NOT NULL ,
    PRIMARY KEY (
        `SensorID`
    )
);

CREATE TABLE `InData` (
    `SensorID` int  NOT NULL ,
    `TimeStamp` time  NOT NULL ,
    `Data` TEXT  NOT NULL 
);

CREATE TABLE `OutData` (
    `SensorID` int  NOT NULL ,
    `TimeStamp` time  NOT NULL ,
    `Data` TEXT  NOT NULL ,
    `updated` int NOT NULL
);

ALTER TABLE `UserGateways` ADD CONSTRAINT `fk_UserGateways_UserID` FOREIGN KEY(`UserID`)
REFERENCES `Users` (`UserID`);

ALTER TABLE `UserGateways` ADD CONSTRAINT `fk_UserGateways_GatewayID` FOREIGN KEY(`GatewayID`)
REFERENCES `Gateways` (`GatewayID`);

ALTER TABLE `Devices` ADD CONSTRAINT `fk_Devices_GatewayID` FOREIGN KEY(`GatewayID`)
REFERENCES `Gateways` (`GatewayID`);

ALTER TABLE `Sensors` ADD CONSTRAINT `fk_Sensors_DeviceID` FOREIGN KEY(`DeviceID`)
REFERENCES `Devices` (`DeviceID`);

ALTER TABLE `InData` ADD CONSTRAINT `fk_InData_SensorID` FOREIGN KEY(`SensorID`)
REFERENCES `Sensors` (`SensorID`);

ALTER TABLE `OutData` ADD CONSTRAINT `fk_OutData_SensorID` FOREIGN KEY(`SensorID`)
REFERENCES `Sensors` (`SensorID`);