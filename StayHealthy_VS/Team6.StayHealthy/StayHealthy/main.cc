#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "registrierung.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StayHealthy w;
    w.show();
    return a.exec();
}