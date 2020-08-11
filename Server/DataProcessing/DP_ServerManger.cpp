#include "DP_ServerManger.h"

#include <QTime>
#include <QDebug>

#include "const_codes.h"
#include "const_users.h"
#include "const_html.h"
//------------------------------------------------------------------------------------------
DP_ServerManager::DP_ServerManager(QObject *parent) :
    QObject(parent),
    _pDP_DbHandler(new DP_DbHandler),
    m_ptcpServer(new QTcpServer())

{
  Q_ASSERT(_pDP_DbHandler->bInit(C_DB_PATH));

  QObject::connect(m_ptcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
}
//------------------------------------------------------------------------------------------
bool DP_ServerManager::bRunServer(uint32_t nPort)
{
  return m_ptcpServer->listen(QHostAddress::Any, nPort);
}
//------------------------------------------------------------------------------------------
void DP_ServerManager::slotNewConnection()
{
  QTcpSocket *pClientSocket = m_ptcpServer->nextPendingConnection();
  QString strMsg = QString("Connected to server!");
  QByteArray bArr;

  ST_PROTOCOL_HEADER st_prot_header;

  // set data
  st_prot_header.uiReqCode = EN_REQUEST_CODE::EN_DEV_CONNECT;
  st_prot_header.uiDataSize+= strMsg.size();
  memcpy(st_prot_header.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);

  // write data
  bArr.setRawData((char*)(&st_prot_header),sizeof(st_prot_header));
  bArr.append(strMsg);

  emit sigMsgSystem(c_strSystemTag,"Detected new device connection! Port: " + QString::number(pClientSocket->peerPort()),EN_MSG_LEVEL_SYSTEM);

  pClientSocket->write(bArr);

  QObject::connect(pClientSocket, SIGNAL(disconnected()),SLOT(slotDeleteConnection()));
  QObject::connect(pClientSocket, SIGNAL(readyRead()),this, SLOT(slotReadClient()));
}
//------------------------------------------------------------------------------------------
void DP_ServerManager::slotReadClient()
{
  QTcpSocket* m_pTcpSocket = dynamic_cast<QTcpSocket*>(sender());
  ST_PROTOCOL_HEADER stHeader;
  QByteArray bArrReceiveData;

  // check data
  while(m_pTcpSocket->bytesAvailable())
  {
    bArrReceiveData.append(m_pTcpSocket->readAll());
  }

  // get header info
  memcpy(&stHeader,bArrReceiveData.data(),sizeof(stHeader));

  if(stHeader.uiUnicLabel == C_UNIC_LABEL)
  {
    switch (stHeader.uiReqCode) {
      case EN_REQUEST_CODE::EN_REQ_CHECK_USER:
      {
        vCheckUserToDb(bArrReceiveData,m_pTcpSocket);
        break;
      }
      case EN_REQUEST_CODE::EN_REQ_ADD_USER:
      {
        vAddNewUserToDb(bArrReceiveData,m_pTcpSocket);
        break;
      }

      case EN_REQUEST_CODE::EN_REQ_SEND_MSG:
      {
        vProcessingMsg(bArrReceiveData,stHeader.uiDataSize);
        break;
      }
      case EN_REQUEST_CODE::EN_DEV_CONNECT:
      {
        vAutorUsersProcessing(bArrReceiveData,m_pTcpSocket);
        break;
      }
      default:
        break;
      }

  }
  else
  {
    qDebug() << "Error 812: Unknow packet!";
  }
}
//------------------------------------------------------------------------------------------
void DP_ServerManager::vCheckUserToDb(QByteArray bArrData,QTcpSocket* pSocket)
{
  uint32_t uiIndentifier(0);
  USER_INFO stUser;
  USER_AUTHOR stAutorUser;
  ST_PROTOCOL_HEADER stHeader;
  QByteArray bArrResData;

  memcpy(&stUser,bArrData.data()+sizeof(ST_PROTOCOL_HEADER),sizeof(stUser));
  memcpy(stHeader.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);

  stHeader.uiDataSize = sizeof(ST_PROTOCOL_HEADER)+sizeof(USER_AUTHOR);
   stHeader.uiReqCode = EN_REQUEST_CODE::EN_REQ_CHECK_USER;
  // check user at the db
  if(_pDP_DbHandler->bCheckUser(stUser,uiIndentifier))
  {
    stAutorUser.uiIndentifier = uiIndentifier;
    stAutorUser.enAccess = EN_ACCESS::EN_VALID;
    stUser.enGroup = _pDP_DbHandler->enGetUserGroup(QString((char*)(stUser.strLogin)));

    _mapAutorsizationUsers.insert(uiIndentifier,T_USER_DATA(stUser,pSocket));

  }
  else
  {
    emit sigMsgSystem(c_strSystemTag,"User with login \"" +QString((char*)stUser.strLogin) + "\" was not found! Device port: " + QString::number(pSocket->peerPort()),EN_MSG_LEVEL_SYSTEM);
  }

  bArrResData.append((char*)(&stHeader),sizeof(stHeader));
  bArrResData.append((char*)(&stAutorUser),sizeof(stAutorUser));

  pSocket->write(bArrResData);
}
//------------------------------------------------------------------------------------------
void DP_ServerManager::vAddNewUserToDb(QByteArray bArrData,QTcpSocket* pSocket)
{
  QByteArray bArrResData;
  USER_INFO stUser;
  QString strSystemMsg;
  ST_PROTOCOL_HEADER stHeader;
  uint32_t uiIndentifier(0);
  USER_AUTHOR stAutorUser;

  memcpy(&stUser,bArrData.data()+sizeof(ST_PROTOCOL_HEADER),sizeof(stUser));
  memcpy(stHeader.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);

  stHeader.uiDataSize = sizeof(ST_PROTOCOL_HEADER)+sizeof(USER_AUTHOR);
  stHeader.uiReqCode = EN_REQUEST_CODE::EN_REQ_ADD_USER;

  // check user at the db
  if(_pDP_DbHandler->bCreateUser(stUser,uiIndentifier))
  {
    stAutorUser.uiIndentifier = uiIndentifier;
    stAutorUser.enAccess = EN_ACCESS::EN_VALID;
    strSystemMsg = "New user registered:" + QString((char*)stUser.strLogin);

  }
  else
  {
    strSystemMsg = "Error: User with User with the login has already existlogin ->" + QString((char*)stUser.strLogin) + " has already exist! Device port: " + QString::number(pSocket->peerPort());
  }

  bArrResData.append((char*)(&stHeader),sizeof(stHeader));
  bArrResData.append((char*)(&stAutorUser),sizeof(stAutorUser));

  pSocket->write(bArrResData);

  emit sigMsgSystem(c_strSystemTag,strSystemMsg,EN_MSG_LEVEL_SYSTEM);
}
//------------------------------------------------------------------------------------------
void DP_ServerManager::vProcessingMsg(QByteArray bArrData,uint32_t uiSize)
{
  USER_INFO stUserInfo;
  USER_AUTHOR stAuthUser;
  QByteArray byteMsgUser;
  uint32_t uiMsgDataSize = uiSize-sizeof(ST_PROTOCOL_HEADER)-sizeof(USER_AUTHOR);


  // parse data
  memcpy(&stAuthUser,bArrData.data()+sizeof(ST_PROTOCOL_HEADER),sizeof(stAuthUser));
  byteMsgUser.append((char*)bArrData.data()+sizeof(ST_PROTOCOL_HEADER)+sizeof(stAuthUser),uiMsgDataSize);

  stUserInfo = std::get<0>(_mapAutorsizationUsers.value(stAuthUser.uiIndentifier));

  emit sigMsgSystem(QString((char*)stUserInfo.strLogin),byteMsgUser,(stUserInfo.enGroup == EN_GROUP::EN_GR_ADMIN ) ? EN_MSG_LEVEL_ADMIN:EN_MSG_LEVEL_USERS);

}
//------------------------------------------------------------------------------------------
void DP_ServerManager::vAutorUsersProcessing(QByteArray bArrData,QTcpSocket* pSocket)
{
  USER_AUTHOR stAuthUser;
  USER_INFO stUserInfo;

  memcpy(&stAuthUser,bArrData.data()+sizeof(ST_PROTOCOL_HEADER),sizeof(stAuthUser));

  stUserInfo = std::get<0>(_mapAutorsizationUsers.value(stAuthUser.uiIndentifier));

  emit sigMsgSystem(c_strSystemTag,QString((char*)stUserInfo.strLogin) + " is connected!",EN_MSG_LEVEL_SYSTEM);

  emit sigSendNetConntections("Device: " + QString((char*)stUserInfo.strLogin),pSocket);

}
//------------------------------------------------------------------------------------------
void DP_ServerManager::slotDeleteConnection()
{
  QTcpSocket* m_pTcpSocket = dynamic_cast<QTcpSocket*>(sender());

  emit sigDeleteConnection(m_pTcpSocket);
}
//------------------------------------------------------------------------------------------

