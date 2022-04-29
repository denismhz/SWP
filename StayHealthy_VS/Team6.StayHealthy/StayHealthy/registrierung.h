#pragma once
#include "qstring.h"
#include "db_handler.h"

class Registrierung {
public:
	static bool ValidiereEmail(QString);
	static bool RegistriereBenutzer(QString, QString);
	static bool BenutzerExistiert(QString);
};