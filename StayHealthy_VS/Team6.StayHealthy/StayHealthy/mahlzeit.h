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
};

