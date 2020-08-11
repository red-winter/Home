#include "client_handler.h"

#include <QTime>
#include <QDataStream>

#include <QDebug>
//------------------------------------------------------------------------------------------
Client_handler::Client_handler(const QString &strHost, int nPort, QObject *parent) :
  QObject(parent),
  m_pTcpSocket(new QTcpSocket())
{
  m_pTcpSocket->connectToHost(strHost, nPort);

  Client_handler::slotDisconnected();

  QObject::connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
  QObject::connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReceiveMsg()));
  QObject::connect(m_pTcpSocket,SIGNAL(disconnected()),SLOT(slotDisconnected()));
  QObject::connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(slotError(QAbstractSocket::SocketError)));
}
//------------------------------------------------------------------------------------------
Client_handler::~Client_handler()
{
  m_pTcpSocket->disconnectFromHost();
  m_pTcpSocket->deleteLater();

  delete m_pTcpSocket;
}
//------------------------------------------------------------------------------------------
void Client_handler::slotSendMsg(QString strCurrentMsg)
{
  QByteArray bArrSendData;
  ST_PROTOCOL_HEADER stHeader;

  stHeader.uiReqCode = EN_RET_CODE::EN_DEV_USER_SEND;
  stHeader.uiDataSize+=strCurrentMsg.size();
  memcpy(stHeader.pTime,QTime::currentTime().toString().toUtf8(),C_TIME_LEN);

  bArrSendData.setRawData((char*)(&stHeader),sizeof(stHeader));
  bArrSendData.append(strCurrentMsg);

  m_pTcpSocket->write(bArrSendData);
}
//------------------------------------------------------------------------------------------
void Client_handler::slotReceiveMsg()
{
  ST_PROTOCOL_HEADER stHeader;
  QByteArray bArrReceiveData;
  QString strMsg = "no_data";

  // read data
  while(m_pTcpSocket->bytesAvailable())
  {
    bArrReceiveData.append(m_pTcpSocket->readAll());
  }

  // get header info
  memcpy(&stHeader,bArrReceiveData.data(),sizeof(stHeader));

  // get msg
  strMsg = bArrReceiveData.remove(0,sizeof(stHeader));

  switch (stHeader.uiReqCode)
  {
    case EN_RET_CODE::EN_DEV_CONNECT:
      emit sigMsgSystem(strMsg);
      break;
    case EN_RET_CODE::EN_DEV_DISCONECT:

      break;
    default:
      break;
  }
}
//------------------------------------------------------------------------------------------
void Client_handler::slotError(QAbstractSocket::SocketError err)
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
void Client_handler::slotConnected()
{
  emit sigStatusConnections(true);
}
//------------------------------------------------------------------------------------------
void Client_handler::slotDisconnected()
{
  emit sigStatusConnections(false);
}
//------------------------------------------------------------------------------------------

