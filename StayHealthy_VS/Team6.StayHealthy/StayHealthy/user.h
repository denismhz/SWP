#pragma once
#ifndef STAYHEALTHY_USER_H_
#define STAYHEALTHY_USER_H_
#include "qstring.h"

class User {
private:
	int id_;

public:
  QString email_;
  QString password_;

  User();
  User(QString email, QString password);
  User(QString email, QString password, int id);
  ~User();

  int GetId();
  void SetId(int id) { id_ = id; }

  //Datenbankfunktionen
  static bool AddUser(User& user);

  static bool DeleteUser(int id);

  static User* GetUser(int id);
  static User* GetUser(QString email, QString password);

  static bool UpdateUser(User& user);
};

#endif //STAYHEALTHY_USER_H_
