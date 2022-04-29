#pragma once
#include "qstring.h"
#include "speiseposition.h"

class Mahlzeit
{
public:
	int id_;
	int kaloriengehalt;
	QString uhrzeit_;
	QString datum_;
	std::vector<Speiseposition*> speisepositionen;

	Mahlzeit();
	~Mahlzeit();
};

