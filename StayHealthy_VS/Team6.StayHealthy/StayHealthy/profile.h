#pragma once
#ifndef STAYHEALTHY_PROFILE_H_
#define STAYHEALTHY_PROFILE_H_
#include <qstring.h>

class Profile {
 public:
  int weight_; //kg
  int height_; //cm
  int grundumsatz_; //kcal
  int kalorienaufnahme_; //kcal
  QString geschlecht_;
  QString name_;
  QString vorname_;
  QString alter_; //datum
  QString prefaerenz_;

  Profile(int, int, QString);
  ~Profile();
};

#endif //STAYHEALTHY_PROFILE_H_
