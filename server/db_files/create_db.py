import mysql.connector as mysql


db = mysql.connect(
    host = "localhost",
    user = "username",
    password = "password",
    database = "domotic"
)

# GRANT ALL PRIVILEGES ON *.* TO 'username'@'localhost' IDENTIFIED BY 'password'; being in sudo mysql

print(db) 

mycursor = db.cursor()
mycursor.execute("CREATE TABLE IF NOT EXISTS `Users` ( \
                `UserID` int  NOT NULL ,\
                `NickName` TEXT NOT NULL ,\
                `Password` varchar(64)  NOT NULL ,\
                `IsAdmin` bool  NOT NULL ,\
                PRIMARY KEY (\
                    `UserID`\
                )\
            );")

f = open("database_create.txt", "r")
mycursor.execute(f.read())