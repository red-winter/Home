#include "DP_ClientManager.h"

#include <QTime>
#include <QDataStream>
#include <QStandardPaths>
#include <QDebug>
//------------------------------------------------------------------------------------------
DP_ClientManager::DP_ClientManager(T_HOST_CONF rConfig, QObject *parent) :
  QObject(parent),
  m_pTcpSocket(new QTcpSocket()),
  _rCurServConfig(rConfig)
{



  qRegisterMetaType<EN_REQUEST_CODE>("EN_REQUEST_CODE");
  qRegisterMetaType<USER_AUTHOR>("USER_AUTHOR");
  qRegisterMetaType<USER_INFO>("USER_INFO");
  qRegisterMetaType<EN_ACCESS>("EN_ACCESS");
  qRegisterMetaType<QVector<T_HOST_CONF>>("QVector<T_HOST_CONF>");

  QObject::connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotGetConnection()));
  QObject::connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReceiveMsgProcessing()));
  QObject::connect(m_pTcpSocket,SIGNAL(disconnected()),SLOT(slotDisconnected()));
  QObject::connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(slotError(QAbstractSocket::SocketError)));
}
//------------------------------------------------------------------------------------------
DP_ClientManager::~DP_ClientManager()
{
  m_pTcpSocket->disconnectFromHost();
  m_pTcpSocket->deleteLater();

  delete m_pTcpSocket;
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotGetConnection()
{
  EN_REQUEST_CODE enRetCode = EN_REQUEST_CODE::EN_DEV_NOTHING;
  QTime tTimeStart = QTime::currentTime();

  tTimeStart.start();
  // connecting to server
  while(!(m_pTcpSocket->waitForConnected()) && (tTimeStart.elapsed() < C_TIME_TO_CONNECT))
  {
    m_pTcpSocket->connectToHost(std::get<0>(_rCurServConfig), std::get<1>(_rCurServConfig));

    m_pTcpSocket->waitForConnected();
  }

  enRetCode = (m_pTcpSocket->state() == QAbstractSocket::ConnectedState) ? EN_REQUEST_CODE::EN_DEV_CONNECT:EN_REQUEST_CODE::EN_DEV_DISCONECT;

  emit sigStatusConnections(enRetCode);
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotCreateNewUser(USER_INFO stUser)
{
  DP_ClientManager::slotCheckUser(stUser,EN_REQ_ADD_USER);
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotCheckUser(USER_INFO stUser, EN_REQUEST_CODE enReqCode)
{
  QByteArray bArrSendData;
  ST_PROTOCOL_HEADER stProtHeader;

  stProtHeader.uiDataSize = sizeof(stProtHeader)+sizeof(stUser);
  stProtHeader.uiReqCode = enReqCode;

  memcpy(stProtHeader.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);

  bArrSendData.append((char*)(&stProtHeader),sizeof(ST_PROTOCOL_HEADER));
  bArrSendData.append((char*)(&stUser),sizeof(stUser));

  m_pTcpSocket->write(bArrSendData);
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotReceiveMsgProcessing()
{
  ST_PROTOCOL_HEADER stHeader;
  QByteArray bArrReceiveData;

  // read data
  while(m_pTcpSocket->bytesAvailable())
  {
    bArrReceiveData.append(m_pTcpSocket->readAll());
  }

  // get header info
  memcpy(&stHeader,bArrReceiveData.data(),sizeof(stHeader));

  // get cmd for packet
  if(stHeader.uiUnicLabel = C_UNIC_LABEL)
  {

    switch (stHeader.uiReqCode)
    {
      case EN_REQUEST_CODE::EN_REQ_CHECK_USER:
      {
        memcpy(&stUniLabel,bArrReceiveData.data()+sizeof(ST_PROTOCOL_HEADER),sizeof(stUniLabel));

        emit sigOpenChatWindow(stUniLabel);

        break;
      }
      case EN_REQUEST_CODE::EN_REQ_ADD_USER:
      {
        USER_AUTHOR stUSerAuthor;
        memcpy(&stUSerAuthor,bArrReceiveData.data()+sizeof(ST_PROTOCOL_HEADER),sizeof(stUSerAuthor));

        emit sigAddNewUser(stUSerAuthor.enAccess);

        break;
      }

      case EN_REQUEST_CODE::EN_REQ_RECEIVE_MSG:
      {
        QString str = "test";
        // TO DO


        emit sigReceivedMsg(str);
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
void DP_ClientManager::slotSendEnteringToSystem()
{
  QByteArray bArrSendData;
  ST_PROTOCOL_HEADER stProtHeader;

  stProtHeader.uiDataSize = sizeof(stProtHeader)+sizeof(stUniLabel);
  stProtHeader.uiReqCode = EN_DEV_CONNECT;

  memcpy(stProtHeader.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);

  stUniLabel.bEntering = true;

  bArrSendData.append((char*)(&stProtHeader),sizeof(ST_PROTOCOL_HEADER));
  bArrSendData.append((char*)(&stUniLabel),sizeof(stUniLabel));

  m_pTcpSocket->write(bArrSendData);
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotDisconnected()
{
  QTcpSocket* m_pTcpSocket = dynamic_cast<QTcpSocket*>(sender());

  emit sigDisconnect(m_pTcpSocket->errorString());
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotError(QAbstractSocket::SocketError err)
{
   QString strError =
       "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                    "The host was not found." :
                    err == QAbstractSocket::RemoteHostClosedError ?
                    "The remote host is closed." :
                    err == QAbstractSocket::ConnectionRefusedError ?
                    "The connection was refused." :
                    QString(m_pTcpSocket->errorString())
                   );

  qDebug() << strError;
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotRequestUsers()
{
  QVector<T_HOST_CONF> vecCurrentUsers;

  vecCurrentUsers.append(T_HOST_CONF("sdas",1));
  vecCurrentUsers.append(T_HOST_CONF("czcz",2));

  // TO DO


  emit sigCurrentUsers(vecCurrentUsers);
}
//------------------------------------------------------------------------------------------
void DP_ClientManager::slotRequestSendMsg(QString strUserMsg)
{
  QByteArray bArrSendData;
  ST_PROTOCOL_HEADER stProtHeader;

  stProtHeader.uiDataSize = sizeof(stProtHeader)+sizeof(stUniLabel)+strUserMsg.size();
  stProtHeader.uiReqCode = EN_REQ_SEND_MSG;

  memcpy(stProtHeader.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);


  bArrSendData.append((char*)(&stProtHeader),sizeof(ST_PROTOCOL_HEADER));
  bArrSendData.append((char*)(&stUniLabel),sizeof(stUniLabel));
  bArrSendData.append(strUserMsg);

  m_pTcpSocket->write(bArrSendData);
}
//------------------------------------------------------------------------------------------
