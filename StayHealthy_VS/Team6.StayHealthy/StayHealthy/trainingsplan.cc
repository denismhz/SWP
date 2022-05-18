#include "trainingsplan.h"
#include "db_handler.h"
#include "registrierung.h"
#include "ernaerungsplan.h"
#include "profil.h"
#include "login.h"

Trainingsplan::Trainingsplan()
{
}

Trainingsplan::Trainingsplan(int user_id, int id, QString start_datum, QString end_datum, QString beschreibung)
	: user_id_(user_id), id_(id), start_datum_(start_datum), end_datum_(end_datum), beschreibung_(beschreibung)
{}

Trainingsplan::~Trainingsplan()
{
	for (auto te : trainingseinheiten_) delete te;
}

void Trainingsplan::ErstelleTrainingsplan(QString start_datum, QString pref)
{
	User* user = Login::GetInstance()->GetLoggedInUser();
	Profil* profil = Profil::GetProfil(*user);

	/*auto today = sys_days{floor<days>(system_clock::now())};

	//datum f�r plan berechnen
	QString p_datum_start = start_datum;
	year_month_day ymd_p_datum_end = sys_days(QStringToYearMonthDate(start_datum)) + days{ 7 };
	QString p_datum_end = ParseDate(ymd_p_datum_end);

	//datum f�r einheiten
	year_month_day ymd_p_datum_start = QStringToYearMonthDate(start_datum);
	QString te_datum_1 = ParseDate(sys_days(ymd_p_datum_start) + days{ 1 });
	QString te_datum_2 = ParseDate(sys_days(ymd_p_datum_start) + days{ 3 });
	QString te_datum_3 = ParseDate(sys_days(ymd_p_datum_start) + days{ 5 });
	std::vector<QString> te_dates{ te_datum_1, te_datum_2, te_datum_3 };

	//datum f�r  get mahlzeiten
	QString m_datum_ende = ParseDate(today);
	QString m_datum_anfang = ParseDate(today - days{ 7 });*/

	//Datum f�r plan
	QDate p_start_datum = QDate::fromString(start_datum, Qt::ISODate);
	QDate p_end_datum = p_start_datum.addDays(7);
	QString sp_start_datum = p_start_datum.toString(Qt::ISODate);
	QString sp_end_datum = p_end_datum.toString(Qt::ISODate);

	//Datum f�r einheiten
	QString te_datum_1 = p_start_datum.addDays(1).toString(Qt::ISODate);
	QString te_datum_2 = p_start_datum.addDays(3).toString(Qt::ISODate);
	QString te_datum_3 = p_start_datum.addDays(5).toString(Qt::ISODate);
	std::vector<QString> te_dates{ te_datum_1, te_datum_2, te_datum_3 };

	//Datum f�r get mahlzeiten
	QString m_datum_ende = QDate::currentDate().toString(Qt::ISODate);
	QString m_datum_start = QDate::currentDate().addDays(-7).toString(Qt::ISODate);

	//Erstelle Trainingsplan ab startdatum
	Trainingsplan* new_plan = new Trainingsplan(user->GetId(), 0,sp_start_datum, sp_end_datum, "Automatisch erstellter Trainingsplan");
	//save Trainingsplan to db so we can get an id of it
	int trp_id = DBHandler::GetInstance()->AddTrainingsplan(*new_plan);
	//get Trainingsplan id
	new_plan->id_ = trp_id;

	std::vector<Mahlzeit*> mahlzeiten;

	int kalorienaufnahme_letzte_7_tage = 0; //kalorien aus mahlzeiten
	int kalorien_pro_einheit = 0;
	int kalorien_pro_uebung = 0;

	//get all mahlzeiten from last week to calculate kalorienaufnahme
	mahlzeiten = DBHandler::GetInstance()->GetMahlzeitenVonBis(user->GetId(), m_datum_start, m_datum_ende);
	if (mahlzeiten.empty()) {
		kalorienaufnahme_letzte_7_tage = profil->kalorienaufnahme_;
		qDebug() << "empty" << kalorienaufnahme_letzte_7_tage;
	}
	else {
		for (Mahlzeit* m : mahlzeiten) {
			kalorienaufnahme_letzte_7_tage += m->GetKaloriengehalt();
		}
	}

	//wieviele kalorien sollen pro �bung bzw. einheit verbraucht werden
	kalorienaufnahme_letzte_7_tage -= profil->GetGrundumsatz() * 7;
	kalorien_pro_einheit = kalorienaufnahme_letzte_7_tage / 3;
	kalorien_pro_uebung = kalorien_pro_einheit / 3;
	qDebug() << "kalorienaufnahme " << kalorienaufnahme_letzte_7_tage << "grundumsatz pro woche " << profil->GetGrundumsatz() << "alter ";

	for (int i = 0; i < 3; i++) {
		//get random uebungen aus datenbank
		Trainingseinheit* t = new Trainingseinheit(0, user->GetId(), trp_id, 0, 0, "", "", te_dates[i]);
		int te_id = DBHandler::GetInstance()->AddTrainingseinheit(*t);
		new_plan->trainingseinheiten_.push_back(t);
		new_plan->trainingseinheiten_[i]->uebungspositionen = DBHandler::GetInstance()->GetRandomUebungen(3, pref);

		//berechne kalorienverbrauch pro uebungsposition
		for (Uebungsposition* b : new_plan->trainingseinheiten_[i]->uebungspositionen) {
			int kalorienverbrauch_pro_uebung = b->kalorienverbrauch_;
			while (b->kalorienverbrauch_ < kalorien_pro_uebung) {
				b->trainingseinheit_id_ = te_id;
				b->menge_++;
				b->kalorienverbrauch_ += kalorienverbrauch_pro_uebung; // += uebung kalorienverbrauch	
			}
			//add Uebungspositionen to db
			DBHandler::GetInstance()->AddUebungsposition(*b);
		}
	}

	//delete
	for (auto m : mahlzeiten) delete m;
	delete new_plan;
}

int Trainingsplan::GetID()
{
	return this->id_;
}

int Trainingsplan::GetKalorienverbrauch()
{
	if (trainingseinheiten_.empty()) return 0;
	int kalorienverbrauch = 0;
	//quadratische laufzeit k�nnte performance bei fro�en pl�nen beeinflussen
	for (auto te : trainingseinheiten_) {
		for (auto up : te->uebungspositionen) {
			kalorienverbrauch += up->kalorienverbrauch_;
		}
	}
	return kalorienverbrauch;
}
