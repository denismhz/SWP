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
	DBHandler::GetInstance()->AddUser(new_user);
	return true;
}

bool Registrierung::BenutzerExistiert(User& check_user)
{
	return DBHandler::GetInstance()->CheckIfEmailExists(check_user.email_);
}

bool Login::LoginUser(User& user)
{
	int email_exists = DBHandler::GetInstance()->CheckIfEmailExists(user.email_);
	User* check_user = DBHandler::GetInstance()->GetUserById(email_exists);
	if (email_exists && !QString::compare(check_user->password_,user.password_, Qt::CaseSensitive)) {
		logged_in_user_ = check_user;
		return true;
	}
	return false;
}

User* Login::GetLoggedInUser()
{
	return this->logged_in_user_;
}
