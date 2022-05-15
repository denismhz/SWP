#include "uebungsposition.h"

Uebungsposition::Uebungsposition()
{
}

Uebungsposition::Uebungsposition(int menge, int uebungs_id, int kalorienverbrauch, int trainingseinheit_id, QString beschreibung)
	: menge_(menge), uebungs_id_(uebungs_id), kalorienverbrauch_(kalorienverbrauch), trainingseinheit_id_(trainingseinheit_id), beschreibung_(beschreibung)
{
}

Uebungsposition::~Uebungsposition()
{
}

int Uebungsposition::KalorienverbrauchBerechnen()
{
	return 0;
}
