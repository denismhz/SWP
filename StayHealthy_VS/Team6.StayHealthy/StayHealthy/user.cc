#include "user.h"
#include "mahlzeit.h"

User::User() {

}

User::User(QString email, QString password) : email_(email), password_(password), id_(0)
{
}

User::User(QString email, QString password, QString name, QString vorname, int geschlecht)
    : email_(email), password_(password), name_(name), vorname_(vorname), geschlecht_(geschlecht),  id_(0) {}

User::User(QString email, QString password, QString name, QString vorname, int geschlecht, int id)
    : email_(email), password_(password), name_(name), vorname_(vorname), geschlecht_(geschlecht), id_(id) {}

User::~User() {}
