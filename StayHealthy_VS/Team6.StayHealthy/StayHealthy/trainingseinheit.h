#pragma once
#include <qstring.h>
#include <vector>
#include "uebungsposition.h"
class Trainingseinheit
{
public:
	int id_;
	int kalorienverbrauch;
	bool durchgefuehrt;
	QString start_zeit_;
	QString end_zeit_;
	QString datum_;
	std::vector<Uebungsposition*> uebungspositionen;

	Trainingseinheit();
	~Trainingseinheit();
};

