#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "const_html.h"
#include "DP_ClientManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

signals:
  void sigCheckResult(EN_REQUEST_CODE enCode);

  void sigChangeCentralLog(QString strMsg);

  void sigEnteringToSystem();

private slots:
  void slotOpenRegistrationForm();

  void slotOpenWelcomForm();

  void slotOpenChatForm(USER_AUTHOR stUserInfo);

  void slotRestartForm(QString strMsg);

  void slotDisconnect(QString strError);
private:

  template <typename T>
  void vCreateWindow(T* pWidget);

  DP_ClientManager *_Client_handler;

  QThread *pThreadHandler;

  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
