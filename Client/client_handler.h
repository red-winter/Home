#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <QObject>
#include <QTcpSocket>

#include "const_codes.h"

class Client_handler : public QObject
{
    Q_OBJECT
public:
  explicit Client_handler(const QString& strHost, int nPort,QObject *parent = nullptr);

  ~Client_handler();
signals:
  void sigStatusConnections(bool bStatus);

  void sigMsgSystem(QString);

private slots:
  void slotConnected();

  void slotDisconnected();


  void slotSendMsg(QString strCurrentMsg);

  void slotReceiveMsg();


  void slotError(QAbstractSocket::SocketError);

private:
  QTcpSocket* m_pTcpSocket;

};

#endif // CLIENT_HANDLER_H
