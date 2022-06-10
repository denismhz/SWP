#pragma once
#include "qstring.h"
#include <vector>

class Uebungsposition
{
public:
	int trainingseinheit_id_;
	int menge_;
	int uebungs_id_;
	int kalorienverbrauch_;
	int id_;
	QString beschreibung_;

	Uebungsposition();
	Uebungsposition(int menge, int uebungs_id, int kalorienverbrauch, int trainingseinheit_id, QString bescreibung);
	~Uebungsposition();

	int KalorienverbrauchBerechnen();

	//DB functions
	static int AddUebungsposition(Uebungsposition&);

	static bool UpdateUebungsposition(Uebungsposition&);

	static std::vector<Uebungsposition*> GetUebungsposition(int trainingseinheit_id);

	static bool DeleteUebungsposition(int id);
};

