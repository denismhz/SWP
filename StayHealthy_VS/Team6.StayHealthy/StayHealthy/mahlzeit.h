#pragma once
#include "qstring.h"
#include "speiseposition.h"

class Mahlzeit
{
public:
	int id_;
	int user_id_;
	int kaloriengehalt_;
	int ernaehrungsplan_id_;
	QString uhrzeit_;
	QString datum_;
	std::vector<Speiseposition*> speisepositionen;

	Mahlzeit();
	~Mahlzeit();

	int GetKaloriengehalt();

	//Datenbankfunktionen
	static std::vector<Mahlzeit*> GetMahlzeit(int user_id);
	static std::vector<Mahlzeit*> GetMahlzeit(int user_id, int eplan_id);
	static std::vector<Mahlzeit*> GetMahlzeit(int user_id, QString start_date, QString end_date);

	static int AddMahlzeit(Mahlzeit&);

	static bool DeleteMahlzeit(int id);
};

