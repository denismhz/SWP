#pragma once

#include "user.h"

class Login {
private:
	User* logged_in_user_;

public:
	static Login* GetInstance() {
		static std::unique_ptr<Login> instance{ new Login() };
		return instance.get();
	}

	bool LoginUser(User&);
	User* GetLoggedInUser();
};