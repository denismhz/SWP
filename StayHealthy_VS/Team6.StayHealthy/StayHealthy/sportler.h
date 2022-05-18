#pragma once
#include "user.h"
#include "qstring.h"
#include "zeitplan.h"
#include "date.h"
#include "date_parser.h"

//irgendwie ist superclass user hier absolut unnötig zumindest wie bis jetz alles ist
//sportler ähnelt mehr einem profil eines users 

class Sportler : public User
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
	Zeitplan* zeitplan_;

	Sportler(int geschlecht, int weight, int height, int kalorienaufnahme, QString geb_datum, QString prefaerenz);
	~Sportler();

	int GetGrundumsatz();
	int GetAlter();
};

