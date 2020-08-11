#include "UI_RegistrationManager.h"
#include "ui_UI_RegistrationManager.h"

#include "const_html.h"
//------------------------------------------------------------------------------------------
#define ERROR_LOG(log)  ui->pLabError->setVisible(true); \
                        ui->pLabError->setText(c_strTemplate.arg(log).arg("#aa0000"));
//------------------------------------------------------------------------------------------
UI_RegistrationManager::UI_RegistrationManager(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::UI_RegistrationManager)
{
  ui->setupUi(this);

  ui->pLabError->setVisible(false);
}
//------------------------------------------------------------------------------------------
UI_RegistrationManager::~UI_RegistrationManager()
{
  delete ui;
}
//------------------------------------------------------------------------------------------
void UI_RegistrationManager::on_pButFromSend_clicked()
{
  QString strLogin = ui->pLeLogin->text();
  QString strPass = ui->pLePass->text();
  EN_SEX_USER enSex = ui->pRbMaleSelect->isChecked() ? EN_SEX_USER::EN_SEX_MALE:EN_SEX_USER::EN_SEX_FAMIN;

  USER_INFO stUser(strLogin,strPass,enSex);

  if(strLogin.isEmpty() || strPass.isEmpty())
  {
    ERROR_LOG("Not correct data into data afields!");
  }
  else
  {
    emit sigRegistration(stUser);
  }
}
//------------------------------------------------------------------------------------------
void UI_RegistrationManager::slotNewUserProcessing(EN_ACCESS enAccess)
{
  if(enAccess == EN_ACCESS::EN_INVALID)
  {
    ERROR_LOG("User with current login already exist!");
  }
  else
  {
    emit sigBack(c_strTemplate.arg("User is created!").arg("#005500"));
  }
}
//------------------------------------------------------------------------------------------
void UI_RegistrationManager::on_pButBack_clicked()
{
  emit sigBack();
}
//------------------------------------------------------------------------------------------
