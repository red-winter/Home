#include "UI_EntetingHandler.h"
#include "ui_UI_EntetingHandler.h"

#include <QMovie>
#include <qDebug>

#define MAC_LOCK(lock) ui->pButConnect->setEnabled(lock); \
                       ui->pButOpenRegistration->setEnabled(lock);
//------------------------------------------------------------------------------------------
UI_EntetingHandler::UI_EntetingHandler(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::UI_EntetingHandler)
{
  ui->setupUi(this);

  ui->pInfoLabel->setVisible(false);

  UI_EntetingHandler::vSetAnimationLoading();
}
//------------------------------------------------------------------------------------------
UI_EntetingHandler::~UI_EntetingHandler()
{
  delete ui;
}
//------------------------------------------------------------------------------------------
void UI_EntetingHandler::on_pButOpenRegistration_clicked()
{
  emit sigOpenRegistationForm();
}
//------------------------------------------------------------------------------------------
void UI_EntetingHandler::on_pButConnect_clicked()
{
  MAC_LOCK(false);

  USER_INFO stUser(ui->pLeLogin->text(),ui->pLePass->text());

  UI_EntetingHandler::vSetAnimationLoading();

  emit sigConnecting(stUser);
}
//------------------------------------------------------------------------------------------
void UI_EntetingHandler::slotResultProcessing(EN_REQUEST_CODE enRetCode)
{
  const QString strLogTemplate= "<html><head/><body><p align=\"center\"><span style=\" color:%2;\">%1</span></p></body></html>";
  QString strLogMsg = strLogTemplate.arg("Error 42: User has not at db!").arg("#aa0000");
  QPixmap oPix(":/new/animation/connect.png");

  if(ui->pLabStatus->movie() != nullptr)
  {
    ui->pLabStatus->movie()->stop();
    ui->pLabStatus->movie()->deleteLater();
  }
  else
  {
    // NOTHING TO DO
  }
  MAC_LOCK(true);

  switch (enRetCode) {
    case EN_REQUEST_CODE::EN_DEV_CONNECT:
    {
      strLogMsg = strLogTemplate.arg("Connecting!").arg("#005500");

      break;
    }
    case EN_REQUEST_CODE::EN_REQ_INVALID_USER:
    {
      oPix.load(":/new/animation/stop.png");
      strLogMsg = strLogTemplate.arg("Error 40: User not found or data in fields not correctly!").arg("#aa0000");

      break;
    }
    case EN_REQUEST_CODE::EN_DEV_DISCONECT:
    {
      oPix.load(":/new/animation/stop.png");
      strLogMsg = strLogTemplate.arg("Error 41: Check out connections!").arg("#aa0000");
      MAC_LOCK(false);
      break;
    }
    default:
    {
      break;
    }
  }

  ui->pLabStatus->setPixmap(oPix.scaled(25,25,Qt::KeepAspectRatio));

  ui->pInfoLabel->setText(strLogMsg);
  ui->pInfoLabel->setVisible(true);
}
//------------------------------------------------------------------------------------------
void UI_EntetingHandler::vSetAnimationLoading()
{
  QMovie *movie = new QMovie(":/new/animation/loading.gif");


  if(ui->pLabStatus->movie() != nullptr)
  {
    ui->pLabStatus->movie()->stop();
    ui->pLabStatus->movie()->deleteLater();
  }
  else
  {
    // NOTHING TO DO
  }

  ui->pLabStatus->setGeometry(QRect(25,25,25,25));
  ui->pLabStatus->setMovie(movie);

  movie->start();
}
//-----------------------------------------------------------------------------------------
void UI_EntetingHandler::slotChangeCentralLog(QString strMsg)
{
  ui->pInfoLabel->setVisible(true);
  ui->pInfoLabel->setText(strMsg);
}
//-----------------------------------------------------------------------------------------
