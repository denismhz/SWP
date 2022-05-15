#include "db_handler.h"
#include "date.h"
#include <strstream>
#include <string>

DBHandler::DBHandler() {
  db_ = QSqlDatabase::addDatabase("QODBC");
  db_.setDatabaseName(
      "DRIVER={SQL "
      "Server};Server=194.95.108.25;Database=StayHealthyDB;Uid=sa;Port=1433;"
      "Pwd=asdasd1!2;WSID=.");
  if (db_.open()) {
    qDebug() << "Database Connected!";
  } else
    qDebug() << "No Database Connected";
}

DBHandler::~DBHandler() {
    db_.close();
}

bool DBHandler::CustomQuery(QString prep){
    qDebug() << prep;
    QSqlQuery query;
    query.prepare(prep);
    query.exec();
    query.finish();
    return 1;
}

bool DBHandler::AddUser(User & new_user) {
  QString prep =
      "INSERT INTO dbo.Sportler (EMail, Password, Nachname, Vorname, Geschlecht) VALUES (:eMail, :password, "
      ":name, :vorname, :geschlecht)";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":eMail", new_user.email_);
  query.bindValue(":password", new_user.password_);
  query.bindValue(":name", new_user.name_);
  query.bindValue(":vorname", new_user.vorname_);
  query.bindValue(":geschlecht", new_user.geschlecht_);
  query.exec();
  query.finish();
  return 1;
}

bool DBHandler::DeleteUserById(int user_id) {
  QString prep = "DELETE FROM dbo.Sportler WHERE BenutzerID = ?";
  QSqlQuery query;
  query.prepare(prep);
  query.addBindValue(user_id);
  query.exec();
  query.finish();
  return 1;
}

bool DBHandler::UpdateUserByID(int user_id, User &new_user) {
  QString prep =
      "UPDATE dbo.Sportler SET EMail = :email, Password = :password"
      " WHERE BenutzerID = :id;";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":weight", new_user.email_);
  query.bindValue(":height", new_user.password_);
  query.bindValue(":id", user_id);
  query.exec();
  query.finish();
  return 1;
}

User *DBHandler::GetUserById(int user_id) {
  QString prep = "SELECT * FROM dbo.Sportler WHERE BenutzerID = :userID";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":userID", user_id);
  query.exec();
  query.first();
  QString email = query.value(1).toString();
  QString password = query.value(2).toString();
  QString name = query.value(5).toString();
  QString vorname = query.value(6).toString();
  int geschlecht = query.value(11).toInt();
  User* user = new User(email, password, name, vorname, geschlecht, user_id);
  query.finish();
  return user;
}

User *DBHandler::GetUserByEmailAndPassword(QString email, QString password) {
  QString prep = "SELECT * FROM dbo.Sportler WHERE EMail = :email AND Password = :password;";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":email", email);
  query.bindValue(":password", password);
  query.exec();
  bool user_exists = query.first();
  int id = query.value(0).toInt();
  QString name = query.value(5).toString();
  QString vorname = query.value(6).toString();
  User *user = new User(email, password, name, vorname, id);
  query.finish();
  if (user_exists)
    return user;
  else
    return 0;
}

//@return id of user with email
int DBHandler::CheckIfEmailExists(QString email) {
  QString prep =
      "SELECT * FROM dbo.Sportler WHERE EMail = :email";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":email", email);
  query.exec();
  if (!query.first()) return false;
  return query.value(0).toInt();
}

Sportler* DBHandler::GetSportlerByUserId(int user_id)
{
    QString prep = "SELECT * FROM dbo.Sportler WHERE BenutzerID = :userID";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":userID", user_id);
    query.exec();
    query.first();
    int weight = query.value(11).toInt();
    int height = query.value(7).toInt();
    int kalorienaufnahme = query.value(9).toInt();
    QString geb_datum = query.value(13).toString();
    QString praeferenz = query.value(12).toString();
    int geschlecht = query.value(10).toInt();
    Sportler* sportler = new Sportler(geschlecht, weight, height, kalorienaufnahme, geb_datum, praeferenz);
    query.finish();
    return sportler;
}

bool DBHandler::UpdateSportlerByUserId(int userID, Sportler &sportler)
{
    QString prep =
        "UPDATE dbo.Sportler SET "
        "EMail = :email, "
        "Password = :password, "
        "Nachname = :nachname, "
        "Vorname = :vorname, "
        "Groesse = :groesse, "
        "Grundumsatz = :grundumsatz, "
        "Kalorienaufnahme = :kalorienaufnahme, "
        "Geschlecht = :geschlecht, "
        "Gewicht = :gewicht, "
        "Praeferenz = :praeferenz, "
        "GebDatum = :gebdatum"
        " WHERE BenutzerID = :id;";


    QSqlQuery query;
    query.prepare(prep);

    query.bindValue(":email", sportler.email_);
    query.bindValue(":password", sportler.password_);
    query.bindValue(":nachname", sportler.name_);
    query.bindValue(":vorname", sportler.vorname_);
    query.bindValue(":groesse", sportler.height_);
    query.bindValue(":grundumsatz", sportler.GetGrundumsatz());
    query.bindValue(":kalorienaufnahme", sportler.kalorienaufnahme_);
    query.bindValue(":geschlecht", sportler.geschlecht_);
    query.bindValue(":gewicht", sportler.weight_);
    query.bindValue(":praeferenz", sportler.prefaerenz_);
    query.bindValue(":gebdatum", sportler.geb_datum_);
    query.bindValue(":id", sportler.GetId());


    qDebug() << "emailDB: " << sportler.email_;

    query.exec();
    query.finish();
    return 1;
}

std::vector<Ernaerungsplan*> DBHandler::GetErnaerungsPlaeneVonBis(int user_id, QString anfang, QString ende)
{
    std::vector<Ernaerungsplan*> plane;
    QString prep = "SELECT * FROM dbo.Ernaehrungsplan WHERE Anfangsdatum >= :anfang AND Enddatum <= :ende";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":anfang", anfang);
    query.bindValue(":ende", ende);
    query.exec();
    while (query.next()) {
        Ernaerungsplan* p = new Ernaerungsplan();
        plane.push_back(p);
    } 
    query.finish();
    return plane;
}

int DBHandler::AddTrainingsplan(Trainingsplan & plan)
{
    QString prep =
        "INSERT INTO dbo.Trainingsplan (BenutzerID, Anfangsdatum, Enddatum) VALUES (:user_id, :anfang, "
        ":ende)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", plan.user_id_);
    query.bindValue(":anfang", plan.start_datum_);
    query.bindValue(":ende", plan.end_datum_);
    query.exec();
    query.finish();
    return query.lastInsertId().toInt();
}

std::vector<Uebungsposition*> DBHandler::GetRandomUebungen(int anzahl, QString art)
{
    std::vector<Uebungsposition*> uebungen;
    QString prep;
    if(!art.isEmpty()) prep = "SELECT TOP(:anzahl) * FROM dbo.Uebung WHERE Art = :art ORDER BY NEWID() ";
    else prep = "SELECT TOP(:anzahl) * FROM dbo.Uebung ORDER BY NEWID()";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":anzahl", anzahl);
    query.bindValue(":art", art);
    query.exec();
    while (query.next()) {
        Uebungsposition* new_pos = new Uebungsposition(1, query.value(0).toInt(), query.value(2).toInt(), 0, query.value(3).toString());
        uebungen.push_back(new_pos);
    }
    query.finish();

    return uebungen;
}

bool DBHandler::AddUebungsposition(Uebungsposition& uebungspos)
{
    QString prep =
        "INSERT INTO dbo.Uebungsposition (TrainingseinheitID, UebungsID, Menge, Beschreibung) VALUES (:te_id, :ue_id, :menge, "
        ":beschreibung)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":te_id", uebungspos.trainingseinheit_id_);
    query.bindValue(":ue_id", uebungspos.uebungs_id_);
    query.bindValue(":menge", uebungspos.menge_);
    query.bindValue(":beschreibung", uebungspos.beschreibung_);
    query.exec();
    query.finish();
    return 1;
}

std::vector<Uebungsposition*> DBHandler::GetUebungsposition(int trainingseinheit_id)
{
    std::vector<Uebungsposition*> ueps;
    QString prep = "select * from Uebungsposition "
        "WHERE TrainingseinheitID = :trainingseinheit_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":trainingseinheit_id", trainingseinheit_id);
    query.exec();
    while (query.next())
    {
        Uebungsposition* ue = new Uebungsposition();
        ue->beschreibung_ = query.value("Beschreibung").toString();
        ue->menge_ = query.value("Menge").toInt();
        ue->uebungs_id_ = query.value("UebungsID").toInt();
        ue->trainingseinheit_id_ = query.value("TrainingseinheitID").toInt();
        ue->id_ = query.value("UebungspositionID").toInt();
        ueps.push_back(ue);
    }
    query.finish();
    return ueps;
}

bool DBHandler::UpdateUebungsposition(int menge, int id)
{
    QString prep = "Update Uebungsposition SET Menge = :menge WHERE UebungspositionID = :uebungs_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":uebungs_id", id);
    query.bindValue(":menge", menge);
    query.exec();
    query.finish();
    return false;
}


std::vector<Trainingseinheit*> DBHandler::GetTrainingseinheit(int user_id)
{
    std::vector<Trainingseinheit*>tein;
    QString prep = "select * from Trainingseinheit "
        "WHERE BenutzerID = :user_id AND Durchgefuehrt = 0";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.exec();
    while (query.next())
    {
        Trainingseinheit* te = new Trainingseinheit();
        te->datum_ = query.value("Datum").toString();
        te->start_zeit_ = query.value("Startzeit").toString();
        te->end_zeit_ = query.value("Endzeit").toString();
        te->durchgefuehrt = query.value("Durchgefuehrt").toBool();
        te->kalorienverbrauch = query.value("Kalorienverbrauch").toInt();
        te->id_ = query.value("TrainingseinheitID").toInt();
        te->uebungspositionen = this->GetUebungsposition(te->id_);
        tein.push_back(te);
    }
    query.finish();
    //get uebungspositionen
    return tein;
}


int DBHandler::AddTrainingseinheit(Trainingseinheit& tein)
{
    QString prep =
        "INSERT INTO dbo.Trainingseinheit (BenutzerID, TrainingsplanID, Startzeit, Endzeit, Datum)"
        " VALUES(:user_id, :t_id, :start, :ende, :datum)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", tein.user_id_);
    query.bindValue(":t_id", tein.trainingsplan_id_);
    query.bindValue(":start", tein.start_zeit_);
    query.bindValue(":ende", tein.end_zeit_);
    query.bindValue(":datum", tein.datum_);
    query.exec();
    query.finish();
    return query.lastInsertId().toInt();
}

std::vector<Mahlzeit*> DBHandler::GetMahlzeitenVonBis(int user_id, QString anfang, QString ende)
{
    std::vector<Mahlzeit*> mahlzeiten;
    QString prep = "SELECT * FROM dbo.Mahlzeit WHERE Datum BETWEEN :start AND :end AND BenutzerID = :user_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":start", anfang);
    query.bindValue(":end", ende);
    query.bindValue(":user_id", user_id);
    query.exec();
    while (query.next()) {
        Mahlzeit* p = new Mahlzeit();
        mahlzeiten.push_back(p);
    }
    query.finish();
    return mahlzeiten;
}

int DBHandler::AddMahlzeit(Mahlzeit& m)
{
    QString prep =
        "INSERT INTO dbo.Mahlzeit (BenutzerID, ErnaehrungsplanID, Uhrzeit, Datum)"
        " VALUES(:user_id, :e_id, :uhrzeit, :datum)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", m.user_id_);
    query.bindValue(":e_id", m.ernaehrungsplan_id_);
    query.bindValue(":uhrzeit", m.uhrzeit_);
    query.bindValue(":datum", m.datum_);
    query.exec();
    query.finish();
    return query.lastInsertId().toInt();
}


