#include "StayHealthy.h"
#include "db_handler.h"
#include "qdebug.h"
#include "main_menu.h"
#include "qmessagebox.h"

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
  MainMenu *m = new MainMenu();
  
  if (!DBHandler::GetInstance()
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
  }
}
