#include "trainingseinheit.h"

Trainingseinheit::Trainingseinheit(int id, int user_id, int tp_id, int calories, bool durch, QString start, QString ende, QString datum)
	: user_id_(user_id), trainingsplan_id_(tp_id), kalorienverbrauch(calories), durchgefuehrt(durch), start_zeit_(start), end_zeit_(ende), datum_(datum), id_(id)
{
}

Trainingseinheit::~Trainingseinheit()
{
}
