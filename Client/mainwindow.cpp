#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "UI_EntetingHandler.h"
#include "UI_RegistrationManager.h"
#include "UI_ChatManager.h"

#include <QTime>
#include <QDebug>
//------------------------------------------------------------------------------------------
#define DELETE_WINDOW(obj)  if(obj != nullptr) \
                            { \
                             delete obj; \
                             setCentralWidget(nullptr); \
                            };
//------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _Client_handler(new DP_ClientManager(T_HOST_CONF(C_HOST_NAME,C_PORT_NUMBER))),
      pThreadHandler(new QThread()),
      ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  _Client_handler->moveToThread(pThreadHandler);

  MainWindow::slotOpenWelcomForm();

  QObject::connect(pThreadHandler, SIGNAL(started()), _Client_handler, SLOT(slotGetConnection()));
  QObject::connect(_Client_handler,SIGNAL(sigOpenChatWindow(USER_AUTHOR)),SLOT(slotOpenChatForm(USER_AUTHOR)));
  QObject::connect(_Client_handler,SIGNAL(sigDisconnect(QString)),SLOT(slotDisconnect(QString)));
  QObject::connect(this,SIGNAL(sigEnteringToSystem()),_Client_handler,SLOT(slotSendEnteringToSystem()));

  pThreadHandler->start();
}
//------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  delete ui;
}
//------------------------------------------------------------------------------------------
void MainWindow::slotOpenRegistrationForm()
{
  UI_RegistrationManager *pWindow = new UI_RegistrationManager();

  vCreateWindow(pWindow);

  QObject::connect(pWindow,SIGNAL(sigRegistration(USER_INFO)),_Client_handler,SLOT(slotCreateNewUser(USER_INFO)));
  QObject::connect(_Client_handler,SIGNAL(sigAddNewUser(EN_ACCESS)),pWindow,SLOT(slotNewUserProcessing(EN_ACCESS)));
  QObject::connect(pWindow,SIGNAL(sigBack(QString)),SLOT(slotRestartForm(QString)));

}
//------------------------------------------------------------------------------------------
void MainWindow::slotOpenWelcomForm()
{
  UI_EntetingHandler *pWindow = new UI_EntetingHandler();

  vCreateWindow(pWindow);

  QObject::connect(pWindow,SIGNAL(sigOpenRegistationForm()),SLOT(slotOpenRegistrationForm()));
  QObject::connect(this,SIGNAL(sigCheckResult(EN_REQUEST_CODE)),pWindow,SLOT(slotResultProcessing(EN_REQUEST_CODE)));

  QObject::connect(pWindow,SIGNAL(sigConnecting(USER_INFO)),_Client_handler,SLOT(slotCheckUser(USER_INFO)));
  QObject::connect(_Client_handler,SIGNAL(sigStatusConnections(EN_REQUEST_CODE)),pWindow,SLOT(slotResultProcessing(EN_REQUEST_CODE)));
  QObject::connect(this,SIGNAL(sigChangeCentralLog(QString)),pWindow,SLOT(slotChangeCentralLog(QString)));

}
//------------------------------------------------------------------------------------------
void MainWindow::slotOpenChatForm(USER_AUTHOR stUserInfo)
{
  UI_EntetingHandler* pCurWidget = qobject_cast<UI_EntetingHandler*>(centralWidget());
  UI_ChatManager *pWindow = new UI_ChatManager();

  QObject::connect(pWindow,SIGNAL(sigUpdateUsers()),_Client_handler,SLOT(slotRequestUsers()));
  QObject::connect(pWindow,SIGNAL(sigSendMsg(QString)),_Client_handler,SLOT(slotRequestSendMsg(QString)));
  QObject::connect(_Client_handler,SIGNAL(sigReceivedMsg(QString)),pWindow,SLOT(slotResponseMsg(QString)));
  QObject::connect(_Client_handler,SIGNAL(sigCurrentUsers(QVector<T_HOST_CONF>)),pWindow,SLOT(vUpdataUsers(QVector<T_HOST_CONF>)));

  if(stUserInfo.enAccess == EN_ACCESS::EN_VALID)
  {
    pCurWidget->slotResultProcessing(EN_REQUEST_CODE::EN_REQ_INVALID_USER);
    vCreateWindow(pWindow);

    emit sigEnteringToSystem();
  }
  else
  {

    if(pCurWidget != nullptr)
    {
      pCurWidget->slotResultProcessing(EN_REQUEST_CODE::EN_REQ_INVALID_USER);
    }
    else
    {
      qDebug() << "Error 12: Have not access!";
    }

    delete pWindow;
  }


}
//------------------------------------------------------------------------------------------
void MainWindow::slotRestartForm(QString strMsg)
{
  MainWindow::slotOpenWelcomForm();

  UI_EntetingHandler *pWindow = qobject_cast<UI_EntetingHandler*>(centralWidget());

  _Client_handler->slotGetConnection();

  if(!strMsg.isEmpty() && (pWindow != nullptr))
  {
    emit sigChangeCentralLog(strMsg);
  }
  else
  {
    // NOTHING TO DO
  }
}
//------------------------------------------------------------------------------------------
void MainWindow::slotDisconnect(QString strError)
{
  MainWindow::slotOpenWelcomForm();

  emit sigChangeCentralLog(c_strTemplate.arg(strError).arg("#aa0000;"));
}
//------------------------------------------------------------------------------------------
template <typename T>
void MainWindow::vCreateWindow(T* pWidget)
{
  QWidget *pCurWindow = centralWidget();

  DELETE_WINDOW(pCurWindow);

  setCentralWidget(pWidget);
}
//------------------------------------------------------------------------------------------
