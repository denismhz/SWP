#pragma once
#ifndef STAYHEALTHY_DB_HANDLER_H_
#define STAYHEALTHY_DB_HANDLER_H_
#include <QtSql>
#include <qstring.h>
#include <vector>
#include "user.h"
#include "ernaerungsplan.h"
#include "date.h"

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
  int CheckIfEmailExists(QString);

  /*<- Profile DB Actions ->*/
  /* bool AddProfileByUserId(int, Profile&);
  bool UpdateProfileByUserId(int, Profile &);
  bool DeleteProfileByUserId(int);
  Profile *GetProfileByUserId(int);*/

  /*<- Ernaerungsplan DB Actions ->*/
  bool AddErnaerungsplan(User &, Ernaerungsplan &);
  bool UpdateErnaerungsplan(User &, Ernaerungsplan &);
  bool DeleteErnaerungsplan(Ernaerungsplan&);
  std::vector<Ernaerungsplan*> GetErnaerungsPlaeneVonBis(int, date::year_month_day, date::year_month_day);

  /*<- Trainingsplan DB Actions ->*/


  /*<- Mahlzeit DB Actions ->*/
  //@param user_id, date_from, date_to
  //dateformat: yyyy-mm-dd
  std::vector<Mahlzeit*> GetMahlzeitenVonBis(int, QString, QString);

  /*<- Trainingseinheit DB Actions ->*/

};

#endif //STAYHEALTHY_DB_HANDLER_H_
