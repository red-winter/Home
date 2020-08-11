#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QListWidgetItem>

#include "const_html.h"
#include "const_codes.h"
#include "DP_ServerManger.h"


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
    void sigSendTestMsg(QTcpSocket*,QString strMsg = "Test");

public slots:
  void slotNewDevice(QString strDevName,QTcpSocket* pSocket);

  void slotSetMsg(QString strLogin, QString strDevName, EN_MSG_LEVEL enLevel = EN_MSG_LEVEL::EN_MSG_LEVEL_SYSTEM);

  void slotDeleteConnection(QTcpSocket* pSocket);

private slots:
  void on_pButtonSendTest_clicked();

private:
  inline QString strGetHtmlLog(QString strMsg);

  DP_ServerManager *_Server;

  QThread *_pMainThread;

  QStringList _listLogMsg;

  Ui::MainWindow *ui;

  QMap<QListWidgetItem*,QTcpSocket*> _mapDevices;
};
#endif // MAINWINDOW_H
