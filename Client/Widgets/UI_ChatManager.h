#ifndef UI_CHATMANAGER_H
#define UI_CHATMANAGER_H

#include <QWidget>

#include "const_users.h"

namespace Ui {
  class UI_ChatManager;
}

class UI_ChatManager : public QWidget
{
  Q_OBJECT

public:
  explicit UI_ChatManager(QWidget *parent = nullptr);

  ~UI_ChatManager();

  void vUpdata() { emit sigUpdateUsers(); }

public slots:
   void vUpdataUsers(QVector<T_HOST_CONF> vecUsers);

   void slotResponseMsg(QString strReceiveMsg);

signals:
  void sigUpdateUsers();

  void sigSendMsg(QString strMsg);

private slots:
  void on_pButSend_clicked();

private:
  inline QString strGetHtmlLog(QString strMsg);

  QStringList _listLogMsg;

  Ui::UI_ChatManager *ui;
};

#endif // UI_CHATMANAGER_H
