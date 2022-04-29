#include "registrierung.h"

bool Registrierung::ValidiereEmail(QString email)
{
	QRegularExpression regex("^[A-Z0-9a-z._-]{1,}@(\\w+)(\\.(\\w+))(\\.(\\w+))?(\\.(\\w+))?$");
	if (regex.match(email).hasMatch()) {
		return true;
	}
	return false;
}

bool Registrierung::RegistriereBenutzer(QString email, QString passwort)
{
	if (!Registrierung::ValidiereEmail(email) || passwort.isEmpty() || DBHandler::GetInstance()->CheckIfEmailExists(email)) {
		return false;
	}
	return true;
}

bool Registrierung::BenutzerExistiert(QString email)
{
	return DBHandler::GetInstance()->CheckIfEmailExists(email);
}
