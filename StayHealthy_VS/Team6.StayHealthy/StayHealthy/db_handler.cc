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

bool DBHandler::AddUser(User & new_user) {
  QString email = new_user.email_;
  QString password = new_user.password_;
  QString prep =
      "INSERT INTO dbo.Sportler (EMail, Password) VALUES (:eMail, :password)";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":eMail", email);
  query.bindValue(":password", password);
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
  QString email = query.value(0).toString();
  QString password = query.value(1).toString();
  User* user = new User(email, password);
  query.finish();
  return user;
}

User *DBHandler::GetUserByEmailAndPassword(QString email, QString password) {
  QString prep = "SELECT * FROM dbo.Sportler WHERE EMail = :email AND Password = :password;";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":email", email);
  query.bindValue(":userID", password);
  query.exec();
  bool user_exists = query.first();
  int id = query.value(0).toInt();
  User *user = new User(email, password, id);
  query.finish();
  if (user_exists)
    return user;
  else
    return 0;
}

bool DBHandler::CheckIfEmailExists(QString email) {
  QString prep =
      "SELECT * FROM dbo.Sportler WHERE EMail = :email";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":email", email);
  query.exec();
  if (!query.first()) return false;
  return true; 
}

bool DBHandler::AddProfileByUserId(int user_id, Profile &new_profile) {
  QString prep =
      "INSERT INTO dbo.Profiles (Weight_, Height_, Pref_, BenutzerID) VALUES (:weight, :height, :pref, :id);";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":weight", new_profile.weight_);
  query.bindValue(":height", new_profile.height_);
  query.bindValue(":pref", new_profile.prefaerenz_);
  query.bindValue(":id", user_id);
  query.exec();
  query.finish();
  return 1;
}

bool DBHandler::UpdateProfileByUserId(int user_id, Profile &new_profile) { 
  QString prep =
      "UPDATE dbo.Profiles SET Weight_ = :weight, Height_ = :height, Pref_ = "
      ":pref WHERE BenutzerID = :id;";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":weight", new_profile.weight_);
  query.bindValue(":height", new_profile.height_);
  query.bindValue(":pref", new_profile.prefaerenz_);
  query.bindValue(":id", user_id);
  query.exec();
  query.finish();
  return 1;
}

bool DBHandler::DeleteProfileByUserId(int user_id) {
  QString prep = "DELETE FROM dbo.Profiles WHERE BenutzerID = ?";
  QSqlQuery query;
  query.prepare(prep);
  query.addBindValue(user_id);
  query.exec();
  query.finish();
  return 0;
}

Profile *DBHandler::GetProfileByUserId(int user_id) {
  QString prep = "SELECT * FROM dbo.Profiles WHERE BenutzerID = :userID";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":userID", user_id);
  query.exec();
  int weight = query.value(0).toInt();
  int height = query.value(1).toInt();
  QString pref = query.value(2).toString();
  Profile *profile = new Profile(weight, height, pref);
  query.finish();
  return profile;
}
