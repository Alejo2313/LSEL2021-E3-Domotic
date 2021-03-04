-- Exported from QuickDBD: https://www.quickdatabasediagrams.com/
-- NOTE! If you have used non-SQL datatypes in your design, you will have to change these here.

-- Modify this code to update the DB schema diagram.
-- To reset the sample schema, replace everything with
-- two dots ('..' - without quotes).

CREATE TABLE `Users` (
    `UserID` int  NOT NULL ,
    `NickName` string  NOT NULL ,
    `Password` varchar(64)  NOT NULL ,
    `IsAdmin` bool  NOT NULL ,
    PRIMARY KEY (
        `UserID`
    )
);

CREATE TABLE `Gateways` (
    `GatewayID` int  NOT NULL ,
    `AdminID` int  NOT NULL ,
    `GatewayKey` varchar(32)  NOT NULL ,
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
    `DevicesID` int  NOT NULL ,
    `GatewayID` int  NOT NULL ,
    `Type` int  NOT NULL ,
    `DataType` int  NOT NULL ,
    PRIMARY KEY (
        `DevicesID`
    )
);

CREATE TABLE `SensorData` (
    `DevicesID` int  NOT NULL ,
    `TimeStamp` date  NOT NULL ,
    `Data` string  NOT NULL 
);

CREATE TABLE `DeviceOrder` (
    `DeviceID` int  NOT NULL ,
    `TimeStamp` date  NOT NULL ,
    `Data` string  NOT NULL 
);

ALTER TABLE `Users` ADD CONSTRAINT `fk_Users_UserID` FOREIGN KEY(`UserID`)
REFERENCES `UserGateways` (`UserID`);

ALTER TABLE `Gateways` ADD CONSTRAINT `fk_Gateways_AdminID` FOREIGN KEY(`AdminID`)
REFERENCES `Users` (`UserID`);

ALTER TABLE `UserGateways` ADD CONSTRAINT `fk_UserGateways_GatewayID` FOREIGN KEY(`GatewayID`)
REFERENCES `Gateways` (`GatewayID`);

ALTER TABLE `Devices` ADD CONSTRAINT `fk_Devices_DevicesID` FOREIGN KEY(`DevicesID`)
REFERENCES `DeviceOrder` (`DeviceID`);

ALTER TABLE `Devices` ADD CONSTRAINT `fk_Devices_GatewayID` FOREIGN KEY(`GatewayID`)
REFERENCES `Gateways` (`GatewayID`);

ALTER TABLE `SensorData` ADD CONSTRAINT `fk_SensorData_DevicesID` FOREIGN KEY(`DevicesID`)
REFERENCES `Devices` (`DevicesID`);

