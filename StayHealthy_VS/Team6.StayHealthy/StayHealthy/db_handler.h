#pragma once
#ifndef STAYHEALTHY_DB_HANDLER_H_
#define STAYHEALTHY_DB_HANDLER_H_
#include <QtSql>
#include <qstring.h>
#include <vector>
#include "user.h"
#include "ernaerungsplan.h"
#include "profil.h"
#include "trainingsplan.h"

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

  //throw exception if query could not be executed!!!!!!!!!!!!!!!!!!!!!!!!!!
  //maybe return id of added things everywhere

  bool CustomQuery(QString);

  /*<- Ernaerungsplan DB Actions ->*/
  std::vector<Ernaerungsplan*> GetErnaerungsPlaeneVonBis(int, QString von, QString bis);

  /*<- Trainingsplan DB Actions ->*/

  //@return id of added Trainingsplan
  int AddTrainingsplan(Trainingsplan &);
  Trainingsplan* GetTrainingsplan();

   /*<- Uebung DB Actions ->*/

  //Select random Uebungen
  std::vector<Uebungsposition*> GetRandomUebungen(int anzahl, QString art);
  //add Uebungsposition
  bool AddUebungsposition(Uebungsposition&);
  std::vector<Uebungsposition*> GetUebungsposition(int trainingseinheit_id);
  bool UpdateUebungsposition(int menge, int id);

  /*<- Trainingseinheit DB Actions ->*/

  //@return Trainingseinheiten where durchgefuehrt = 0
  std::vector<Trainingseinheit*> GetTrainingseinheit(int user_id);

  //@return id of added Trainingseinheit
  int AddTrainingseinheit(Trainingseinheit&);

  /*<- Mahlzeit DB Actions ->*/
  //@param user_id, date_from, date_to
  std::vector<Mahlzeit*> GetMahlzeitenVonBis(int, QString von, QString bis);
  int AddMahlzeit(Mahlzeit&);

  int AddSpeiseposition(Speiseposition &, int mahlzeit_id);
};

#endif //STAYHEALTHY_DB_HANDLER_H_
