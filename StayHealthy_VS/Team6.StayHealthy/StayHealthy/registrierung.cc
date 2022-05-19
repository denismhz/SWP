#include "registrierung.h"

bool Registrierung::ValidiereEmail(QString email)
{
	QRegularExpression regex("^[A-Z0-9a-z._-]{1,}@(\\w+)(\\.(\\w+))(\\.(\\w+))?(\\.(\\w+))?$");
	if (regex.match(email).hasMatch()) {
		return true;
	}
	return false;
}

bool Registrierung::RegistriereBenutzer(User& new_user)
{
	if (!Registrierung::ValidiereEmail(new_user.email_) || CheckIfEmailExists(new_user.email_)) {
		return false;
	}
	User::AddUser(new_user);
	return true;
}

bool Registrierung::BenutzerExistiert(User& check_user)
{
	return CheckIfEmailExists(check_user.email_);
}

bool Registrierung::CheckIfEmailExists(QString& email)
{
	QString prep =
		"SELECT * FROM dbo.Benutzer WHERE EMail = :email";
	QSqlQuery query;
	query.prepare(prep);
	query.bindValue(":email", email);
	query.exec();
	if (!query.first()) return false;
	return true;
}
