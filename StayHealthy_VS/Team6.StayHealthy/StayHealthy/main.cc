#include "stayhealthy.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StayHealthy w;
    w.show();
    return a.exec();
}

// todo:    parse y/m/d to d/m/y
//          get current week begin and end date 
//          mehod QString to year_month_date anb year_month_date to QString
