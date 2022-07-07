#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "db_handler.h"

int main(int argc, char* argv[])
{
    
    QApplication a(argc, argv);
    DBHandler::GetInstance();
    StayHealthy w;
    w.show();
    return a.exec();
}
