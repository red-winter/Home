#ifndef DP_CLIENT_MANAGER_H
#define DP_CLIENT_MANAGER_H

#include <QObject>
#include <QTcpSocket>

#include "const_codes.h"
#include "const_users.h"


class DP_ClientManager : public QObject
{
    Q_OBJECT
public:
  explicit DP_ClientManager(T_HOST_CONF rConfig,QObject *parent = nullptr);

  ~DP_ClientManager();

public slots:
  void slotGetConnection();

  void slotCreateNewUser(USER_INFO stUser);

  void slotCheckUser(USER_INFO stUser,EN_REQUEST_CODE enReqCode = EN_REQUEST_CODE::EN_REQ_CHECK_USER);

  void slotDisconnected();

  void slotRequestUsers();

  void slotRequestSendMsg(QString strUserMsg);

  void slotSendEnteringToSystem();

signals:
  void sigStatusConnections(EN_REQUEST_CODE enStatus);

  void sigOpenChatWindow(USER_AUTHOR);

  void sigAddNewUser(EN_ACCESS enAccess);

  void sigCurrentUsers(QVector<T_HOST_CONF> vecUsers);

  void sigReceivedMsg(QString strReceiveMsg);

  void sigDisconnect(QString strError);

private slots:
  void slotReceiveMsgProcessing();

  void slotError(QAbstractSocket::SocketError);

private:
  USER_AUTHOR stUniLabel;

  QTcpSocket* m_pTcpSocket;

  T_HOST_CONF _rCurServConfig;

};

#endif // DP_CLIENT_MANAGER_H
