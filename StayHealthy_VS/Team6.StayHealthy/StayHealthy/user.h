#pragma once
#ifndef STAYHEALTHY_USER_H_
#define STAYHEALTHY_USER_H_
#include "qstring.h"

class User {
 public:
  QString email_;
  QString password_;
  QString name_;
  QString vorname_;
  int geschlecht_;
  int id_;

  User();
  User(QString, QString);
  User(QString, QString, QString, QString, int);
  User(QString, QString, QString, QString, int, int);
  ~User();
};

#endif //STAYHEALTHY_USER_H_
