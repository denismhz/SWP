#pragma once
#include <vector>
#include <qstring.h>
#include <trainingseinheit.h>

class Trainingsplan
{
public:
	int id_;
	int kalorienverbrauch;
	QString start_datum_;
	QString end_dataum;
	QString beschreibung;
	std::vector<Trainingseinheit*> trainingseinheiten;

	Trainingsplan();
	~Trainingsplan();
};