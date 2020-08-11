#ifndef UI_REGISTRATIONMANAGERR_H
#define UI_REGISTRATIONMANAGERR_H

#include <QWidget>

#include "const_users.h"

namespace Ui {
  class UI_RegistrationManager;
}

class UI_RegistrationManager : public QWidget
{
  Q_OBJECT

public:
  explicit UI_RegistrationManager(QWidget *parent = nullptr);

  ~UI_RegistrationManager();

public slots:
  void slotNewUserProcessing(EN_ACCESS);

signals:
  void sigRegistration(USER_INFO);

  void sigBack(QString strMsg = "");

private slots:
  void on_pButBack_clicked();

  void on_pButFromSend_clicked();

private:
  Ui::UI_RegistrationManager *ui;
};

#endif // UI_REGISTRATIONMANAGERR_H
