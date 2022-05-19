#pragma once
#include <QtSql>
#include "qstring.h"
#include "user.h"

class Registrierung {
public:
	static bool ValidiereEmail(QString);
	static bool RegistriereBenutzer(User&);
	static bool BenutzerExistiert(User&);
	static bool CheckIfEmailExists(QString&);
};

