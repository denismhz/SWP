#include "db_handler.h"

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

int DBHandler::AddUser(User & new_user) {
  QString email = new_user.email_;
  QString password = new_user.password_;
  if (email.isEmpty() || password.isEmpty()) return 0;
  QString prep =
      "INSERT INTO dbo.Benutzer (EMail, Passwort) VALUES (:eMail, :password)";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":eMail", email);
  query.bindValue(":password", password);
  query.exec();
  query.finish();
  return 1;
}

int DBHandler::DeleteUserById(int user_id) {
  QString prep = "DELETE FROM dbo.Benutzer WHERE BenutzerID = ?";
  QSqlQuery query;
  query.prepare(prep);
  query.addBindValue(user_id);
  query.exec();
  query.finish();
  return 0;
}

int DBHandler::UpdateUserByID(int, User &) { return 0; }

User *DBHandler::GetUserById(int user_id) {
  User *user = nullptr;
  QString prep = "SELECT * FROM dbo.Benutzer WHERE BenutzerID = :userID";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":userID", user_id);
  query.exec();
  // qDebug() << "query length: " << query.numRowsAffected() << "\n";
  //while (query.next()) {
    // qDebug() << "User found with id: " << query.value(2).toString() << "\n";
  //}
  user = new User(query.value(0).toString(), query.value(1).toString());
  query.finish();
  return user;
}

int DBHandler::AddProfileByUserId(int user_id, Profile &new_profile) {
  QString prep =
      "INSERT INTO dbo.Profiles (Weight_, Height_, Pref_, BenutzerID) VALUES (:weight, :height, :pref, :id);";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":weight", new_profile.weight_);
  query.bindValue(":height", new_profile.height_);
  query.bindValue(":pref", new_profile.pref_);
  query.bindValue(":id", user_id);
  query.exec();
  query.finish();
  return 1;
}

int DBHandler::UpdateProfileByUserId(int user_id, Profile &new_profile) { 
  QString prep =
      "UPDATE dbo.Profiles SET Weight_ = :weight, Height_ = :height, Pref_ = "
      ":pref WHERE BenutzerID = :id;";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":weight", new_profile.weight_);
  query.bindValue(":height", new_profile.height_);
  query.bindValue(":pref", new_profile.pref_);
  query.bindValue(":id", user_id);
  query.exec();
  query.finish();
  return 1;
}

int DBHandler::DeleteProfileByUserId(int user_id) {
  QString prep = "DELETE FROM dbo.Profiles WHERE BenutzerID = ?";
  QSqlQuery query;
  query.prepare(prep);
  query.addBindValue(user_id);
  query.exec();
  query.finish();
  return 0;
}

Profile *DBHandler::GetProfileByUserId(int user_id) {
  Profile *profile = nullptr;
  QString prep = "SELECT * FROM dbo.Profiles WHERE BenutzerID = :userID";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":userID", user_id);
  query.exec();
  // qDebug() << "query length: " << query.numRowsAffected() << "\n";
  // while (query.next()) {
  // qDebug() << "User found with id: " << query.value(2).toString() << "\n";
  //}
  
  profile = new Profile(query.value(0).toInt(), query.value(1).toInt(),
                        query.value(2).toString());
  query.finish();
  return profile;
}
