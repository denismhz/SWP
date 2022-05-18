#include "user.h"
#include <QtSql>
#include "mahlzeit.h"

User::User(QString email, QString password) : email_(email), password_(password), id_(0)
{
}

User::User(QString email, QString password, int id) : email_(email), password_(password), id_(id)
{
}

User::~User() {}

int User::GetId()
{
    return this->id_;
}

//Datenbankfunktionen
bool User::AddUser(User& user)
{
    QString prep =
        "INSERT INTO dbo.Benutzer (EMail, Password) VALUES (:eMail, :password)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":eMail", user.email_);
    query.bindValue(":password", user.password_);
    query.exec();
    query.finish();
    return 1;
}

bool User::DeleteUser(int id)
{
    QString prep = "DELETE FROM dbo.Benutzer WHERE BenutzerID = ?";
    QSqlQuery query;
    query.prepare(prep);
    query.addBindValue(id);
    query.exec();
    query.finish();
    return 1;
}

User* User::GetUser(int id)
{
    QString prep = "SELECT * FROM dbo.Benutzer WHERE BenutzerID = :userID";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":userID", id);
    query.exec();
    query.first();
    QString email = query.value(1).toString();
    QString password = query.value(2).toString();
    QString name = query.value(5).toString();
    QString vorname = query.value(6).toString();
    int geschlecht = query.value(11).toInt();
    User* user = new User(email, password, id);
    query.finish();
    return user;
}

User* User::GetUser(QString email, QString password)
{
    QString prep = "SELECT * FROM dbo.Benutzer WHERE EMail = :email AND Password = :password;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":email", email);
    query.bindValue(":password", password);
    query.exec();
    int id = query.value(0).toInt();
    User* user = new User(email, password, id);
    query.finish();
    return user;
}

bool User::UpdateUser(User& user)
{
    QString prep =
        "UPDATE dbo.Benutzer "
        "SET Email = :email, password = :password "
        "WHERE BenutzerID = :id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":eMail", user.email_);
    query.bindValue(":password", user.password_);
    query.bindValue(":id", user.GetId());
    query.exec();
    query.finish();
    return 1;
}
