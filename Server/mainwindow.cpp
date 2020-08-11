#include "mainwindow.h"
#include "ui_mainwindow.h"



#include <QTime>
#include <QDebug>
//------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _Server(new DP_ServerManager(this)),
      _pMainThread(new QThread()),
      ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  Q_ASSERT(_Server->bRunServer(C_PORT_NUMBER));


  ui->pTeLogTable->setText(strGetHtmlLog(
                             c_strMsgTemplate.arg(QTime::currentTime().toString()).arg("Server is started!").arg("#aa0000").arg("System")
                             )
                           );

  _Server->moveToThread(_pMainThread);

  QObject::connect(_Server,SIGNAL(sigSendNetConntections(QString,QTcpSocket*)),SLOT(slotNewDevice(QString,QTcpSocket*)));
  QObject::connect(_Server,SIGNAL(sigMsgSystem(QString,QString,EN_MSG_LEVEL)),SLOT(slotSetMsg(QString,QString,EN_MSG_LEVEL)));
  QObject::connect(_Server,SIGNAL(sigDeleteConnection(QTcpSocket*)),SLOT(slotDeleteConnection(QTcpSocket*)));
  _pMainThread->start();
}
//------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//------------------------------------------------------------------------------------------
void MainWindow::on_pButtonSendTest_clicked()
{
  ui->pTeLogTable->setText(strGetHtmlLog(
                             c_strMsgTemplate.arg(QTime::currentTime().toString()).arg("Send test request!").arg("#aa0000").arg(c_strSystemTag)
                             )

                           );

}
//------------------------------------------------------------------------------------------
void MainWindow::slotNewDevice(QString strDevName, QTcpSocket *pSocket)
{
  QListWidgetItem *pItem = new QListWidgetItem(strDevName);

  ui->pListConnections->addItem(pItem);

  _mapDevices.insert(pItem,pSocket);
}
//------------------------------------------------------------------------------------------
void MainWindow::slotSetMsg(QString strLogin, QString strMsg, EN_MSG_LEVEL enLevel)
{
  QString strColor = "#0000ff";

  switch (enLevel) {
    case EN_MSG_LEVEL::EN_MSG_LEVEL_SYSTEM:
      strColor = "#aa0000";
      break;

    case EN_MSG_LEVEL::EN_MSG_LEVEL_USERS:
      strColor = "##005500";
      break;
    default:
      break;
    }

  ui->pTeLogTable->setText(strGetHtmlLog(
                             c_strMsgTemplate.arg(QTime::currentTime().toString()).arg(strMsg).arg(strColor).arg(strLogin)
                             )
                           );

}
//------------------------------------------------------------------------------------------
QString MainWindow::strGetHtmlLog(QString strMsg)
{
   QString strMainHtml;

  _listLogMsg.append(strMsg);

  std::for_each(_listLogMsg.begin(),_listLogMsg.end(),[&strMainHtml](QString it){ strMainHtml+=it; });

  return strMainHtml;
}
//------------------------------------------------------------------------------------------
void MainWindow::slotDeleteConnection(QTcpSocket* pSocket)
{
  QListWidgetItem *pItem = _mapDevices.key(pSocket,nullptr);
  QString strUserName = pItem->text();

  if(pItem != nullptr)
  {
    strUserName.remove("Device: ");

    ui->pTeLogTable->setText(strGetHtmlLog(
                               c_strMsgTemplate.arg(QTime::currentTime().toString()).
                                                arg(strUserName + " has left! Port: " + QString::number(pSocket->peerPort())).
                                                arg("#aa0000").arg(c_strSystemTag)
                               )

                             );

    delete pItem;

    pSocket->deleteLater();
  }
  else
  {
    qDebug() << "Error 82: Socket not found!";
  };

}
//------------------------------------------------------------------------------------------
