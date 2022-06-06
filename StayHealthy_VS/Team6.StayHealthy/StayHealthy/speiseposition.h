#pragma once
#include <qstring.h>

class Speiseposition
{
public:
	int id_;
	int menge_;
	int speise_id_;
	int mahlzeit_id_;
	int kaloriengehalt_;
	QString beschreibung_;

	Speiseposition();
	~Speiseposition();

	//Datenbankfunktionen
	static std::vector<Speiseposition*> GetSpeiseposition(int mahlzeit_id);

	static int AddSpeiseposition(Speiseposition&);

	static int DeleteSpeiseposition(int id);
};

