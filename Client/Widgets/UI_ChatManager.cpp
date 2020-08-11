#include "UI_ChatManager.h"
#include "ui_UI_ChatManager.h"

#include "const_html.h"

#include <QTime>
#include <QDebug>
//------------------------------------------------------------------------------------------
UI_ChatManager::UI_ChatManager(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::UI_ChatManager)
{
  ui->setupUi(this);
}
//------------------------------------------------------------------------------------------
UI_ChatManager::~UI_ChatManager()
{
  delete ui;
}
//------------------------------------------------------------------------------------------
void UI_ChatManager::on_pButSend_clicked()
{
  const QString strSendMsg = ui->pTeMessage->toPlainText();
  QString strHtmlLog = strGetHtmlLog(c_strMsgTemplate.arg(QTime::currentTime().toString()).arg(strSendMsg).arg("#005500").arg(" Me"));

  ui->pTeInfoLog->setHtml(c_strChatMsgTemplate.arg(strHtmlLog));

  ui->pTeMessage->clear();

  emit sigSendMsg(strSendMsg);
}
//------------------------------------------------------------------------------------------
void UI_ChatManager::vUpdataUsers(QVector<T_HOST_CONF> vecUsers)
{
  auto funAdd = [this](T_HOST_CONF it)
  {
    QListWidgetItem *pItem = new QListWidgetItem(std::get<0>(it));

    ui->pListUsers->addItem(pItem);
  };

  std::for_each(vecUsers.begin(),vecUsers.end(),funAdd);
}
//------------------------------------------------------------------------------------------
void UI_ChatManager::slotResponseMsg(QString strReceiveMsg)
{
  QString strHtmlLog = strGetHtmlLog(c_strMsgTemplate.arg(QTime::currentTime().toString()).arg(strReceiveMsg).arg("#aa0000").arg(" ??"));

  ui->pTeInfoLog->setPlainText(strHtmlLog);
}
//------------------------------------------------------------------------------------------
QString UI_ChatManager::strGetHtmlLog(QString strMsg)
{
   QString strMainHtml;

  _listLogMsg.append(strMsg);

  std::for_each(_listLogMsg.begin(),_listLogMsg.end(),[&strMainHtml](QString it){ strMainHtml+=it; });

  return strMainHtml;
}
//------------------------------------------------------------------------------------------
