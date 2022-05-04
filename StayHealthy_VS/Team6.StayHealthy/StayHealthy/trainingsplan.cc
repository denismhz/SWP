#include "trainingsplan.h"
#include "db_handler.h"
#include "registrierung.h"
#include "ernaerungsplan.h"
#include "date.h"
#include "date_parser.h"

using namespace std::chrono;
using namespace date;
using namespace dateparser;

Trainingsplan::Trainingsplan()
{
}

Trainingsplan::~Trainingsplan()
{
}

Trainingsplan* Trainingsplan::ErstelleTrainingsplan()
{
	User* user = Login::GetInstance()->logged_in_user_;
	//Profile* profil = DBHandler::GetInstance()->GetProfileByUserId(user->id_);
	Trainingsplan* new_plan;
	std::vector<Mahlzeit*> mahlzeiten;
	std::vector<Trainingseinheit*> einheiten;
	//int kalorienaufnahme_letzte_7_tage = profil->grundumsatz_; //grundumsatz + kalorien aus mahlzeiten
	int kalorien_pro_einheit;
	int kalorien_pro_uebung;
	//get today date and today - 7 days date
	auto today = sys_days{ floor<days>(system_clock::now()) };
	auto last_week_start = today - days{7};
	QString p_today = ParseDate(today);
	QString p_last_week_start = ParseDate(last_week_start);
	//get all mahlzeiten from last week to calculate kalorienaufnahme
	mahlzeiten = DBHandler::GetInstance()->GetMahlzeitenVonBis(user->id_, p_last_week_start, p_today);

	qDebug() << mahlzeiten.size();
	return nullptr;
}
