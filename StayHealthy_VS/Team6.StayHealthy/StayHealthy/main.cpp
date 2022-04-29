#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "qstring.h"
#include "db_handler.h"
#include "user.h"
#include "profile.h"

bool ValidateEmail(QString);
bool registrieren(QString, QString);

int main(int argc, char *argv[])
{
  
    QApplication a(argc, argv);
    //wenn keine verbindung zur datenbank hergestellt werden kann geht nichts, kein window wird angezeigt usw.
    //auto db = DBHandler::GetInstance();
    qDebug() << registrieren("asd@asd.de", "skdfdlfkaj");
    User *new_user = new User("asd@asd.de", "skdfdlfkaj");
    if(registrieren(new_user->email_, new_user->password_)) DBHandler::GetInstance()->AddUser(*new_user);
    StayHealthy w;
    w.show();
    return a.exec();
}

bool registrieren(QString email, QString password) {
  if (!ValidateEmail(email) || password.isEmpty() || DBHandler::GetInstance()->CheckIfEmailExists(email)) {
    return false;
  }
  return true;
}

bool ValidateEmail(QString email) {
  QRegularExpression regex("^[A-Z0-9a-z._-]{1,}@(\\w+)(\\.(\\w+))(\\.(\\w+))?(\\.(\\w+))?$");
  if (regex.match(email).hasMatch()) {
    return true;
  }
  return false;
}