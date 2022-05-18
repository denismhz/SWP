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

