#pragma once
#include <vector>
#include <qstring.h>
#include <mahlzeit.h>

class Ernaerungsplan
{
public:
	int id_;
	int kaloriengehalt;
	QString start_datum_;
	QString end_dataum; //braucht man das hier oder ist ein plan immer eine woche
	QString beschreibung;
	std::vector<Mahlzeit*> mahlzeiten;

	Ernaerungsplan();
	~Ernaerungsplan();
}; 