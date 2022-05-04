#pragma once
#include "user.h"
#include "qstring.h"
#include "zeitplan.h"
#include "date.h"

class Sportler : public User
{
public:
	int weight_; //kg
	int height_; //cm
	int grundumsatz_; //kcal
	int kalorienaufnahme_; //kcal startwert durchschnittliche aufnahme pro woche von sportler angegeben
	int alter_;
	QString geschlecht_;
	QString prefaerenz_;
	Zeitplan* zeitplan_;

	Sportler();
	~Sportler();

	//@param gewicht, groeﬂe in cm, alter, geschlecht: "m" or "w";
	int GrundUmsatzBerechnen(int gewicht, int groesse, int alter, QString geschlecht);
};

