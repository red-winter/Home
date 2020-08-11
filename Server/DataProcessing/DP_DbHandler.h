#ifndef DP_DBHANDLER_H
#define DP_DBHANDLER_H

#include <QObject>
#include <QMap>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

#include "const_users.h"
#include "const_sql.h"


class DP_DbHandler : public QObject
{
  Q_OBJECT
public:
  explicit DP_DbHandler(QObject *parent = 0);

  bool bInit(const QString strPath);

  bool bCheckUser(USER_INFO &stUser, uint32_t &uiIndentifier);

  bool bCreateUser(USER_INFO &stUser, uint32_t &uiIndentifier);

  EN_GROUP enGetUserGroup(QString strLogin);
};

#endif // DP_DBHANDLER_H
