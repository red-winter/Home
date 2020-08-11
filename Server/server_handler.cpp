#include "server_handler.h"

#include <QDebug>
#include <QMessageBox>
#include <QTime>
//------------------------------------------------------------------------------------------
Server_handler::Server_handler(QObject *parent) :
    m_ptcpServer(new QTcpServer()),
    QObject(parent)
{
  QObject::connect(m_ptcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
}
//------------------------------------------------------------------------------------------
bool Server_handler::bRunServer(uint32_t nPort)
{
  return m_ptcpServer->listen(QHostAddress::Any, nPort);
}
//------------------------------------------------------------------------------------------
void Server_handler::slotNewConnection()
{
  QTcpSocket *pClientSocket = m_ptcpServer->nextPendingConnection();
  QString strMsg = QString("Connected to server!");
  QByteArray bArr;

  ST_PROTOCOL_HEADER st_prot_header;

  // set data
  st_prot_header.uiReqCode = EN_RET_CODE::EN_DEV_CONNECT;
  st_prot_header.uiDataSize+= strMsg.size();
  memcpy(st_prot_header.pTime,QTime::currentTime().toString().toUtf8().data(),C_TIME_LEN);

  // write data
  bArr.setRawData((char*)(&st_prot_header),sizeof(st_prot_header));
  bArr.append(strMsg);

  sendToClient(pClientSocket, bArr);

  emit sigSendNetConntections("New dev ",pClientSocket);

  QObject::connect(pClientSocket, SIGNAL(disconnected()),pClientSocket, SLOT(deleteLater()));
  QObject::connect(pClientSocket, SIGNAL(readyRead()),this, SLOT(slotReadClient()));
}
//------------------------------------------------------------------------------------------
void Server_handler::slotReadClient()
{
  QTcpSocket* m_pTcpSocket = dynamic_cast<QTcpSocket*>(sender());
  ST_PROTOCOL_HEADER stHeader;
  QString strMsg;
  QByteArray bArrReceiveData;

  // check data
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
    case EN_RET_CODE::EN_DEV_USER_SEND:
      emit sigMsgSystem(strMsg);
      break;
    default:
      break;
  }



}
//------------------------------------------------------------------------------------------
void Server_handler::slotSendMsg(QTcpSocket* pSocket,QString strMsg)
{
  qDebug() << strMsg << (pSocket != nullptr);

   sendToClient(pSocket,strMsg.toUtf8());
}
//------------------------------------------------------------------------------------------
void Server_handler::sendToClient(QTcpSocket* pSocket, const QByteArray& msg)
{
//  QByteArray  arrBlock(msg);
//  QDataStream out(&arrBlock, QIODevice::WriteOnly);
//  out.setVersion(QDataStream::Qt_4_2);

  pSocket->write(msg);
}
//------------------------------------------------------------------------------------------
