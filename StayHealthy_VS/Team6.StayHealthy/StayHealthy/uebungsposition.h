#pragma once
#include "qstring.h"

class Uebungsposition
{
public:
	int menge_;
	int uebungs_id;
	int kalorienverbrauch_;
	QString beschreibung_;

	Uebungsposition();
	~Uebungsposition();
};

