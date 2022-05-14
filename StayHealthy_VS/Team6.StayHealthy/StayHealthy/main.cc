#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "user.h"
#include "registrierung.h"
#include "db_handler.h"

int main(int argc, char *argv[])
{
    //User* curr = DBHandler::GetInstance()->GetUserById(4);
    //Login::GetInstance()->LoginUser(*curr);
    QApplication a(argc, argv);
    StayHealthy w;
    w.show();
    return a.exec();
}
