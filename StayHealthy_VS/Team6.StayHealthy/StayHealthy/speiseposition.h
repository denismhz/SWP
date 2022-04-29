#pragma once
#include <qstring.h>

class Speiseposition
{
public:
	int menge_;
	int speise_id_;
	int kaloriengehalt;
	QString beschreibung_;

	Speiseposition();
	~Speiseposition();
};

