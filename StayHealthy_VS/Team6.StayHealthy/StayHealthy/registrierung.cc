#include "registrierung.h"
#include "user.h"
#include "db_handler.h"

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
	if (!Registrierung::ValidiereEmail(new_user.email_) || DBHandler::GetInstance()->CheckIfEmailExists(new_user.email_)) {
		return false;
	}
	User::AddUser(new_user);
	return true;
}

bool Registrierung::BenutzerExistiert(User& check_user)
{
	return DBHandler::GetInstance()->CheckIfEmailExists(check_user.email_);
}
