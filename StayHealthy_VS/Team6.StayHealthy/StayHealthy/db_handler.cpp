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
  if (!query.exec()) {
    return 0;
  }
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

int DBHandler::UpdateUserByID(int user_id, User &new_user) {
  QString prep =
      "UPDATE dbo.Benutzer SET EMail = :email, password = :password"
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
  
  QString prep = "SELECT * FROM dbo.Benutzer WHERE BenutzerID = :userID";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":userID", user_id);
  if (!query.exec()) {
    return 0;
  }
  // qDebug() << "query length: " << query.numRowsAffected() << "\n";
  //while (query.next()) {
    // qDebug() << "User found with id: " << query.value(2).toString() << "\n";
  //}
  query.first();
  QString email = query.value(0).toString();
  QString password = query.value(1).toString();
  User* user = new User(email, password);
  query.finish();
  return user;
}

User *DBHandler::GetUserByEmailAndPassword(QString email, QString password) {
  QString prep = "SELECT * FROM dbo.Benutzer WHERE EMail = :email AND Passwort = :password;";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":email", email);
  query.bindValue(":userID", password);
  if (!query.exec()) {
    return 0;
  }
  // qDebug() << "query length: " << query.numRowsAffected() << "\n";
  // while (query.next()) {
  // qDebug() << "User found with id: " << query.value(2).toString() << "\n";
  //}
  bool user_exists = query.first();
  int id = query.value(0).toInt();
  User *user = new User(email, password, id);
  query.finish();
  if (user_exists)
    return user;
  else
    return 0;
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
  
  QString prep = "SELECT * FROM dbo.Profiles WHERE BenutzerID = :userID";
  QSqlQuery query;
  query.prepare(prep);
  query.bindValue(":userID", user_id);
  query.exec();
  // qDebug() << "query length: " << query.numRowsAffected() << "\n";
  // while (query.next()) {
  // qDebug() << "User found with id: " << query.value(2).toString() << "\n";
  //}
  int weight = query.value(0).toInt();
  int height = query.value(1).toInt();
  QString pref = query.value(2).toString();
  
  Profile *profile = new Profile(weight, height, pref);
  query.finish();
  return profile;
}
