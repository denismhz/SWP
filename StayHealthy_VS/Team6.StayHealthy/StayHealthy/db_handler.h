#pragma once
#ifndef STAYHEALTHY_DB_HANDLER_H_
#define STAYHEALTHY_DB_HANDLER_H_
#include <QtSql>
#include <qstring.h>
#include "user.h"
#include "profile.h"

/*<- Singleton ->*/
class DBHandler {
 public:
  QSqlDatabase db_;

  static DBHandler* GetInstance() {
    static std::unique_ptr<DBHandler> instance{new DBHandler()};
    return instance.get();
  }

  DBHandler();
  ~DBHandler();

  /*<- User DB Actions ->*/
  bool AddUser(User &);
  bool DeleteUserById(int);
  bool UpdateUserByID(int, User &);
  User* GetUserById(int);
  User* GetUserByEmailAndPassword(QString, QString);
  bool CheckIfEmailExists(QString);

  /*<- Profile DB Actions ->*/
  bool AddProfileByUserId(int, Profile &);
  bool UpdateProfileByUserId(int, Profile &);
  bool DeleteProfileByUserId(int);
  Profile *GetProfileByUserId(int);

  /*<- Profile DB Actions ->*/

};

#endif //STAYHEALTHY_DB_HANDLER_H_
