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
  User* user = new User(email, password, name, vorname, user_id);
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

std::vector<Ernaerungsplan*> DBHandler::GetErnaerungsPlaeneVonBis(int user_id, date::year_month_day anfang, date::year_month_day ende)
{
    std::vector<Ernaerungsplan*> plane;
    std::stringstream stream;
    stream << "'" << anfang << "' ";
    QString s = QString::fromUtf8(stream.str());
    QString prep = "SELECT * FROM dbo.Ernaehrungsplan WHERE Anfangsdatum >";
    prep += s;
    stream.str(std::string());
    stream.clear();
    stream << " '" << ende << "'";
    qDebug() << QString::fromUtf8(stream.str());
    s = QString::fromUtf8(stream.str());
    prep += "AND Enddatum <";
    prep += s;
    QSqlQuery query;
    query.prepare(prep);
    query.exec();
    while (query.next()) {
        qDebug() << query.value(0).toString() << "!!!!!!!!";
        Ernaerungsplan* p = new Ernaerungsplan();
        plane.push_back(p);
    } 
    query.finish();
    qDebug() << prep;
    return plane;
}

std::vector<Mahlzeit*> DBHandler::GetMahlzeitenVonBis(int user_id, QString anfang, QString ende)
{
    std::vector<Mahlzeit*> mahlzeiten;
    QString prep = "SELECT * FROM dbo.Mahlzeit WHERE Datum BETWEEN :start AND :end AND BenutzerID = :user_id";
    qDebug() << anfang << ende;
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":start", anfang);
    query.bindValue(":end", ende);
    query.bindValue(":user_id", user_id);
    query.exec();
    qDebug() << user_id;
    while (query.next()) {
        qDebug() << query.value(0).toString() << "!!!!!!!!";
        Mahlzeit* p = new Mahlzeit();
        mahlzeiten.push_back(p);
    }
    qDebug() << query.lastQuery();
    query.finish();
    return mahlzeiten;
}
