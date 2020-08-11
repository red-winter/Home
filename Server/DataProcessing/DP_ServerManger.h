#ifndef DP_SERVERMANAGER_H
#define DP_SERVERMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include "const_codes.h"

#include "DP_DbHandler.h"

typedef std::pair<USER_INFO,QTcpSocket*> T_USER_DATA;

class DP_ServerManager : public QObject
{
  Q_OBJECT
public:
  explicit DP_ServerManager(QObject *parent = nullptr);

  bool  bRunServer(uint32_t uiPort);

signals:
  void sigSendNetConntections(QString,QTcpSocket*);

  void sigMsgSystem(QString,QString,EN_MSG_LEVEL);

  void sigDeleteConnection(QTcpSocket*);

public slots:
  virtual void slotNewConnection();

  void slotReadClient();

private slots:
  void slotDeleteConnection();

private:
  void vCheckUserToDb(QByteArray bArrData,QTcpSocket* pSocket);

  void vAddNewUserToDb(QByteArray bArrData,QTcpSocket* pSocket);

  void vProcessingMsg(QByteArray bArrData, uint32_t uiSize);

  void vAutorUsersProcessing(QByteArray bArrData,QTcpSocket* pSocket);

  DP_DbHandler *_pDP_DbHandler;

  QTcpServer* m_ptcpServer;

  QMap<uint32_t,T_USER_DATA> _mapAutorsizationUsers;

};

#endif // DP_SERVERMANAGER_H
