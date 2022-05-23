#pragma once
#include <vector>
#include <qstring.h>
#include <mahlzeit.h>

class Ernaehrungsplan
{
private:
	int id_;
	int kaloriengehalt;

	int BerechneKaloriengehalt();

public:
	QString start_datum_;
	QString end_dataum; //braucht man das hier oder ist ein plan immer eine woche
	int user_id_;
	std::vector<Mahlzeit*> mahlzeiten;

	Ernaehrungsplan();
	~Ernaehrungsplan();

	int GetID();
	void SetID(int id);
	int GetKaloriengehalt();

	//Datenbank funktionen
	static Ernaehrungsplan* GetErnaehrungsplan(int user_id, QString start_datum);
	static std::vector<Ernaehrungsplan*> GetErnaehrungsplan(int user_id);

	static int AddErnaehrungsplan(Ernaehrungsplan&);

	static bool DeleteErnaehrungsplan(int id);
}; 