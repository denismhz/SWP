#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "user.h"
#include "login.h"
#include "registrierung.h"
#include "db_handler.h"

int main(int argc, char* argv[])
{
    DBHandler::GetInstance();
    User* user = User::GetUser(3);
    Login::GetInstance()->LoginUser(*user);
    QApplication a(argc, argv);
    StayHealthy w;
    w.show();
    return a.exec();
}
