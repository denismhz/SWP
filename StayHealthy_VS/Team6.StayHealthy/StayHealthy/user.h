#pragma once
#ifndef STAYHEALTHY_USER_H_
#define STAYHEALTHY_USER_H_
#include <qstring.h>

class User {
 public:
  QString email_;
  QString password_;
  int id_;

  User(QString, QString);
  ~User();
};

#endif //STAYHEALTHY_USER_H_
