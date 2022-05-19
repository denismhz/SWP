#include "login.h"
#include "db_handler.h"
#include "registrierung.h"

bool Login::LoginUser(User& user)
{
	bool email_exists = Registrierung::CheckIfEmailExists(user.email_);
	User* check_user = User::GetUser(user.email_);
	qDebug() << user.password_ << check_user->password_;
	if (email_exists && !QString::compare(check_user->password_, user.password_, Qt::CaseSensitive)) {
		logged_in_user_ = check_user;
		return true;
	}
	delete check_user;

	return false;
}

User* Login::GetLoggedInUser()
{
	return this->logged_in_user_;
}