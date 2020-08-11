#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

#include "const_codes.h"

class Server_handler : public QObject
{
  Q_OBJECT
public:
  explicit Server_handler(QObject *parent = nullptr);

  bool  bRunServer(uint32_t uiPort);

signals:
  void sigSendNetConntections(QString,QTcpSocket*);

  void sigMsgSystem(QString);

public slots:
  virtual void slotNewConnection();

  void slotSendMsg(QTcpSocket* pSocket,QString strMsg);

  void slotReadClient();

private:
  QTcpServer* m_ptcpServer;

  void sendToClient(QTcpSocket* pSocket, const QByteArray &msg);

};

#endif // SERVER_HANDLER_H
