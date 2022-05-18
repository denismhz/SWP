#pragma once
#include "qstring.h"
#include <QtSql>
#include "user.h"

//irgendwie ist superclass user hier absolut unnötig zumindest wie bis jetz alles ist
//sportler ähnelt mehr einem profil eines users 

class Profil
{
private:
	int grundumsatz_; //kcal
	int alter_;

	//@param gewicht, groeße in cm, alter, geschlecht: "m" : 1 or "w" : 0;
	int GrundumsatzBerechnen(int gewicht, int groesse, int alter, int geschlecht);
	int AlterBerechnen(QString geb_datum);

public:
	int weight_; //kg
	int height_; //cm
	int kalorienaufnahme_; //kcal startwert durchschnittliche aufnahme pro woche von sportler angegeben
	int geschlecht_;
	
	QString name_;
	QString vorname_;
	QString prefaerenz_;
	QString geb_datum_;

	Profil();
	~Profil();

	int GetGrundumsatz();
	int GetAlter();

	//Datenbankfunktionen
	static bool AddProfil(Profil& profil, int id);

	static Profil* GetProfil(int id);
	static Profil* GetProfil(User& user);

	static bool UpdateProfil(Profil& profil, int id);
};

