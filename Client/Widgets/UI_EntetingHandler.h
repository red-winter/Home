#ifndef UI_REGISTRATIONHANDLER_H
#define UI_REGISTRATIONHANDLER_H

#include <QWidget>

#include "const_codes.h"
#include "const_users.h"

namespace Ui {
  class UI_EntetingHandler;
}

class UI_EntetingHandler : public QWidget
{
  Q_OBJECT

public:
  explicit UI_EntetingHandler(QWidget *parent = 0);

  ~UI_EntetingHandler();

signals:
  void sigOpenRegistationForm();

  void sigConnecting(USER_INFO);

public slots:
  void slotResultProcessing(EN_REQUEST_CODE enRetCode);

  void slotChangeCentralLog(QString strMsg);

private slots:
  void on_pButOpenRegistration_clicked();

  void on_pButConnect_clicked();

private:
  inline void vSetAnimationLoading();

  Ui::UI_EntetingHandler *ui;
};

#endif // UI_REGISTRATIONHANDLER_H
