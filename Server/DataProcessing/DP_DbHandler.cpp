#include "DP_DbHandler.h"

#include "ByteOperation.h"
#include <QCryptographicHash>
#include <QDebug>
//------------------------------------------------------------------------------------------
DP_DbHandler::DP_DbHandler(QObject *parent) : QObject(parent)
{

}
//------------------------------------------------------------------------------------------
bool DP_DbHandler::bInit(const QString strPath)
{
  QSqlDatabase db;
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(strPath);

  db.open();

  return db.open();
}
//------------------------------------------------------------------------------------------
bool DP_DbHandler::bCheckUser(USER_INFO&stUser,uint32_t& uiIndentifier)
{
  bool bSucces = false;
  QString strLogin = QString((char*)stUser.strLogin);
  QSqlQuery query;

  query.exec(sqlGetCount.arg(strSqlTagUsers).arg(strSqlTagLogin).arg(strLogin));
  query.next();

  bSucces = (query.value(0).toInt() > 0);

  uiIndentifier = bSucces ? qChecksum((char*)stUser.strLogin,C_LEN_USER_PARAM):0;

  return bSucces;
}
//------------------------------------------------------------------------------------------
EN_GROUP DP_DbHandler::enGetUserGroup(QString strLogin)
{
  EN_GROUP enGroup = EN_GROUP::EN_GR_OTHERS;
  QSqlQuery query;

  query.exec(sqlGetGroup.arg(strSqlTagGroup).arg(strSqlTagUsers).arg(strSqlTagLogin).arg(strLogin));
  query.next();

  enGroup = static_cast<EN_GROUP>(query.value(0).toInt());

  return enGroup;
}
//------------------------------------------------------------------------------------------
bool DP_DbHandler::bCreateUser(USER_INFO&stUser,uint32_t& uiIndentifier)
{
  QString strLogin = QString((char*)stUser.strLogin);
  QString strPass = QString((char*)stUser.strPass);
  QSqlQuery query;

  bool bSucces = bCheckUser(stUser,uiIndentifier) != true;

  if(bSucces)
  {
    bSucces = query.exec(sqlInserUser.arg(strSqlTagUsers).arg(strLogin).arg(strPass).arg(EN_GROUP::EN_GR_OTHERS));
    query.next();
  }
  else
  {
    // NOTHING TO DO
  }

  return bSucces;
}
//------------------------------------------------------------------------------------------
