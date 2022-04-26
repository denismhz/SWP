#include "user.h"

User::User(QString email, QString password)
    : email_(email), password_(password), id_(0) {}

User::User(QString email, QString password, int id)
    : email_(email), password_(password), id_(id) {}

User::~User() {}
