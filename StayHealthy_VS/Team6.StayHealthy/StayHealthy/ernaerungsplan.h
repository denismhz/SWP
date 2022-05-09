#pragma once
#include <vector>
#include <qstring.h>
#include <mahlzeit.h>

class Ernaerungsplan
{
private:
	int id_;
	int kaloriengehalt;

	int BerechneKaloriengehalt();

public:
	QString start_datum_;
	QString end_dataum; //braucht man das hier oder ist ein plan immer eine woche
	QString beschreibung;
	std::vector<Mahlzeit*> mahlzeiten;

	Ernaerungsplan();
	~Ernaerungsplan();

	int GetID();
	int GetKaloriengehalt();
}; 