#include "user.h"
#include "mahlzeit.h"

User::User(QString email, QString password) : email_(email), password_(password), name_(""), vorname_(""), id_(0), geschlecht_(0)
{
}

User::User(QString email, QString password, QString name, QString vorname, int geschlecht)
    : email_(email), password_(password), name_(name), vorname_(vorname), geschlecht_(geschlecht),  id_(0) {}

User::User(QString email, QString password, QString name, QString vorname, int geschlecht, int id)
    : email_(email), password_(password), name_(name), vorname_(vorname), geschlecht_(geschlecht), id_(id) {}

User::User()
{
}

User::~User() {}

int User::GetId()
{
    return this->id_;
}
