#ifndef CONST_SQL_H
#define CONST_SQL_H

#include <QString>

#define C_DB_PATH "c:/dreamforest/project/Server/res/server_user_db.db"

const QString sqlGetLogin = "SELECT %1.%2 FROM %1 WHERE %1.%2 = \"%3\"";
const QString sqlGetGroup = "SELECT %1 FROM %2 WHERE %3 = \"%4\"";
const QString sqlGetCount = "SELECT COUNT(%2) FROM %1 WHERE %1.%2 = \"%3\"";
const QString sqlInserUser = "INSERT INTO %1 VALUES(\"%2\",\"%3\",%4)";
const QString sqlGetUserInfo = "SELECT * FROM %2 WHERE %3 = \"%1\"";

const QString strSqlTagLogin = "LOGIN";
const QString strSqlTagUsers = "USERS";
const QString strSqlTagGroup= "UGROUP";

#endif // CONST_SQL_H
