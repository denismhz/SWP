#pragma once
#include "qstring.h"
#include "db_handler.h"
#include "user.h"

class Registrierung {
public:
	static bool ValidiereEmail(QString);
	static bool RegistriereBenutzer(User&);
	static bool BenutzerExistiert(User&);
};

class Login {
public:
	User* logged_in_user_;

	static Login* GetInstance() {
		static std::unique_ptr<Login> instance{ new Login() };
		return instance.get();
	}

	bool LoginUser(User&);

};