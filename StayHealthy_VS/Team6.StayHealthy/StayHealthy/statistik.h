#pragma once
#include <qstring.h>

class Statistik {
public:
	int abgeschlossene_T_einheiten_;
	int abgeschlossene_E_plaene;
	int kalorienaufnahme;
	int kalorienverbrauch;
	QString start_datum_;
	QString end_datum_;
	QString beschreibung;

	Statistik();
	~Statistik();
};