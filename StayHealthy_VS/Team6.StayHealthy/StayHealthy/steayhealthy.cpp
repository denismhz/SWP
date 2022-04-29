#include "stayhealthy.h"
#include "db_handler.h"
#include "qdebug.h"
#include "qmessagebox.h"
#include "tests_stacked.h"

StayHealthy::StayHealthy(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
  
    connect(ui.register_button, SIGNAL(clicked()), this, SLOT(Register()));
}

void StayHealthy::Register() {
  QString password = ui.password_line_edit->text();
  QString email = ui.email_line_edit->text();
  User *new_user = new User(email, password);
  TestsStackedClass *m = new TestsStackedClass();
  QMainWindow::setCentralWidget(m);
  //QMainWindow::setCentralWidget(menu);
  //DBHandler::GetInstance()->AddUser(*new_user);
  //widgets->setCurrentIndex(0);
  
  /* if (!DBHandler::GetInstance()
          ->GetUserByEmailAndPassword(email, password) && DBHandler::GetInstance()
          ->AddUser(*new_user)) {
    qDebug() << "user added";
    QMainWindow::setCentralWidget(m);
  } else {
    QMessageBox user_already_exists;
    user_already_exists.setText(
        "EMail already registered! Choose other EMail!");
    user_already_exists.exec();
    ui.email_line_edit->clear();
    ui.password_line_edit->clear();
    qDebug() << "user already exists";
    return;
  }*/


  //Ist Email gültig?
  //Ist Passwort ungleich null
  //Ist Email schon in der Datenbank?
}





