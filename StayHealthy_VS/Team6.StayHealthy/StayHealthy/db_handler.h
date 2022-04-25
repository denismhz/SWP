#pragma once
#ifndef STAYHEALTHY_DB_HANDLER_H_
#define STAYHEALTHY_DB_HANDLER_H_
#include <QtSql>
#include <qstring.h>
#include "user.h"
#include "profile.h"

class DBHandler {
 public:
  QSqlDatabase db_;
  DBHandler();
  ~DBHandler();
  int AddUser(User &);
  int DeleteUserById(int);
  int UpdateUserByID(int, User &);
  User* GetUserById(int);

  int AddProfileByUserId(int, Profile &);
  int UpdateProfileByUserId(int, Profile &);
  int DeleteProfileByUserId(int);
  Profile *GetProfileByUserId(int);
};

#endif //STAYHEALTHY_DB_HANDLER_H_
