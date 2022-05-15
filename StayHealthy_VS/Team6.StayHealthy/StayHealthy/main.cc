#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "db_handler.h"
#include "user.h"
#include "registrierung.h"

int main(int argc, char *argv[])
{
    User* user = DBHandler::GetInstance()->GetUserById(4);
    Login::GetInstance()->LoginUser(*user);
    DBHandler::GetInstance();
    QApplication a(argc, argv);
    StayHealthy w;
    w.show();
    return a.exec();
}
