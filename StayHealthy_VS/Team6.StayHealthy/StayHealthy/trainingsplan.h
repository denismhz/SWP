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
	
	static void ErstelleTrainingsplan(QString start_datum, QString pref);
	int GetID();
	void SetID(int id);
	int GetKalorienverbrauch();

	//DB functions
	static int AddTrainingsplan(Trainingsplan&);

	static bool UpdateTrainingsplan(Trainingsplan&);

	static std::vector<Trainingsplan*> GetTrainingsplan(int user_id);
	static std::vector<Trainingsplan*> GetTrainingsplan(int user_id, QString begin_date);
	static std::vector<Trainingsplan*> GetTrainingsplan(int user_id, QString begin_date, QString end_date);

	static bool DeleteTrainingsplan(int id);
};