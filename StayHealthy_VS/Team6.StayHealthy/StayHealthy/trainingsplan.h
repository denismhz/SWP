#pragma once
#include <vector>
#include <qstring.h>
#include <trainingseinheit.h>

class Trainingsplan
{
private:
	int id_;

public:
	
	int user_id_;
	QString start_datum_;
	QString end_datum_;
	QString beschreibung_;
	std::vector<Trainingseinheit*> trainingseinheiten_;

	Trainingsplan();
	Trainingsplan(int user_id, int id, QString start_datum, QString end_datum, QString beschreibung);
	~Trainingsplan();
	
	void ErstelleTrainingsplan(QString start_datum, QString pref);
	int GetID();
	int GetKalorienverbrauch();
};