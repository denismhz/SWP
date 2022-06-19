#pragma once
#include <qstring.h>
#include <vector>
#include "uebungsposition.h"
class Trainingseinheit
{
public:
	int id_;
	int user_id_;
	int trainingsplan_id_;
	int kalorienverbrauch;
	bool durchgefuehrt;
	QString start_zeit_;
	QString end_zeit_;
	QString datum_;
	std::vector<Uebungsposition*> uebungspositionen;

	Trainingseinheit();
	Trainingseinheit(int, int, int, bool, QString, QString, QString);
	~Trainingseinheit();

	//DB funtions
	static int AddTrainingseinheit(Trainingseinheit&);

	static std::vector<Trainingseinheit*> GetTrainingseinheit(int user_id);
	static std::vector<Trainingseinheit*> GetTrainingseinheit(int user_id, int trainingsplan_id);
	static std::vector<Trainingseinheit*> GetTrainingseinheit(int user_id, QString datum);

	static bool DeleteTrainingseinheit(int id);
};

