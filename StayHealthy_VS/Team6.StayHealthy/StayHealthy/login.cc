#include "login.h"
#include "db_handler.h"

bool Login::LoginUser(User& user)
{
	int email_exists = DBHandler::GetInstance()->CheckIfEmailExists(user.email_);
	User* check_user = User::GetUser(email_exists);
	if (email_exists && !QString::compare(check_user->password_, user.password_, Qt::CaseSensitive)) {
		logged_in_user_ = check_user;
		return true;
	}
	return false;
}

User* Login::GetLoggedInUser()
{
	return this->logged_in_user_;
}