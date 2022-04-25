#include "stayhealthy.h"
#include <QtWidgets/QApplication>
#include "qstring.h"
#include "db_handler.h"
#include "user.h"
#include "profile.h"

int main(int argc, char *argv[])
{
  
    QApplication a(argc, argv);

    DBHandler *db = new DBHandler;
    User *u = new User("asd@asd.de", "asd");
    //db->AddUser(*u);
    Profile *p = new Profile(1, 2, "hier");
    db->AddProfileByUserId(11, *p);
    p = new Profile(12222, 325235, "llllalala");
    //db->UpdateProfileByUserId(6, *p);
    //db->DeleteProfileByUserId(6);
    //db->DeleteUserById(6);
    delete (p);
    //delete (u);
    StayHealthy w;
    w.show();
    return a.exec();
}
