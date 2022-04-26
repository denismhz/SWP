#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "qstring.h"
#include "db_handler.h"
#include "user.h"
#include "profile.h"

int main(int argc, char *argv[])
{
  
    QApplication a(argc, argv);
    auto db = DBHandler::GetInstance();
    StayHealthy w;
    w.show();
    return a.exec();
}