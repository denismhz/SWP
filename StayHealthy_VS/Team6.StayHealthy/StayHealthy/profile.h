#pragma once
#ifndef STAYHEALTHY_PROFILE_H_
#define STAYHEALTHY_PROFILE_H_
#include <qstring.h>

class Profile {
 public:
  int weight_;
  int height_;
  QString pref_;

  Profile(int, int, QString);
  ~Profile();

};

#endif //STAYHEALTHY_PROFILE_H_
