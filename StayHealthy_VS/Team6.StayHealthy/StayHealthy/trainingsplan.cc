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

	//Datum für plan
	QDate p_start_datum = QDate::fromString(start_datum, Qt::ISODate);
	QDate p_end_datum = p_start_datum.addDays(7);
	QString sp_start_datum = p_start_datum.toString(Qt::ISODate);
	QString sp_end_datum = p_end_datum.toString(Qt::ISODate);

	//Datum für einheiten
	QString te_datum_1 = p_start_datum.addDays(1).toString(Qt::ISODate);
	QString te_datum_2 = p_start_datum.addDays(3).toString(Qt::ISODate);
	QString te_datum_3 = p_start_datum.addDays(5).toString(Qt::ISODate);
	std::vector<QString> te_dates{ te_datum_1, te_datum_2, te_datum_3 };

	//Datum für get mahlzeiten
	QString m_datum_ende = QDate::currentDate().toString(Qt::ISODate);
	QString m_datum_start = QDate::currentDate().addDays(-7).toString(Qt::ISODate);

	//Erstelle Trainingsplan ab startdatum
	Trainingsplan* new_plan = new Trainingsplan(user->GetId(), 0,sp_start_datum, sp_end_datum, "Automatisch erstellter Trainingsplan");

	std::vector<Mahlzeit*> mahlzeiten;

	int kalorienaufnahme_letzte_7_tage = 0; //kalorien aus mahlzeiten
	int kalorien_pro_einheit = 0;
	int kalorien_pro_uebung = 0;

	//get all mahlzeiten from last week to calculate kalorienaufnahme
	mahlzeiten = Mahlzeit::GetMahlzeit(user->GetId(), m_datum_start, m_datum_ende);
	if (mahlzeiten.empty()) {
		kalorienaufnahme_letzte_7_tage = profil->kalorienaufnahme_;
		qDebug() << "empty" << kalorienaufnahme_letzte_7_tage;
	}
	else {
		for (Mahlzeit* m : mahlzeiten) {
			kalorienaufnahme_letzte_7_tage += m->GetKaloriengehalt();
			qDebug() << m->GetKaloriengehalt();
			qDebug() << kalorienaufnahme_letzte_7_tage << ":letzte 7";
		}
	}

	//wieviele kalorien sollen pro übung bzw. einheit verbraucht werden
	kalorienaufnahme_letzte_7_tage -= profil->GetGrundumsatz() * 7;
	kalorien_pro_einheit = kalorienaufnahme_letzte_7_tage / 3;
	kalorien_pro_uebung = kalorien_pro_einheit / 3;
	qDebug() << "kalorienaufnahme " << kalorienaufnahme_letzte_7_tage << "grundumsatz pro woche " << profil->GetGrundumsatz() << "alter ";

	for (int i = 0; i < 3; i++) {
		//get random uebungen aus datenbank
		Trainingseinheit* t = new Trainingseinheit(0, user->GetId(), kalorien_pro_einheit, 0, "", "", te_dates[i]);
		t->uebungspositionen = DBHandler::GetInstance()->GetRandomUebungen(3, pref);

		//berechne kalorienverbrauch pro uebungsposition
		for (Uebungsposition* b : t->uebungspositionen) {
			int kalorienverbrauch_pro_uebung = b->kalorienverbrauch_;
			while (b->kalorienverbrauch_ < kalorien_pro_uebung) {
				b->menge_++;
				b->kalorienverbrauch_ += kalorienverbrauch_pro_uebung; // += uebung kalorienverbrauch	
			}
		}
		new_plan->trainingseinheiten_.push_back(t);
	}

	Trainingsplan::AddTrainingsplan(*new_plan);

	//delete
	//for (auto m : mahlzeiten) delete m;
	//delete new_plan;
}

int Trainingsplan::GetID()
{
	return this->id_;
}

int Trainingsplan::GetKalorienverbrauch()
{
	if (trainingseinheiten_.empty()) return 0;
	int kalorienverbrauch = 0;
	//quadratische laufzeit könnte performance bei froßen plänen beeinflussen
	for (auto te : trainingseinheiten_) {
		for (auto up : te->uebungspositionen) {
			kalorienverbrauch += up->kalorienverbrauch_;
		}
	}
	return kalorienverbrauch;
}

int Trainingsplan::AddTrainingsplan(Trainingsplan& plan)
{
	QString prep =
		"INSERT INTO dbo.Trainingsplan (BenutzerID, Anfangsdatum, Enddatum) VALUES (:user_id, :anfang, "
		":ende)";
	QSqlQuery query;
	query.prepare(prep);
	query.bindValue(":user_id", plan.user_id_);
	query.bindValue(":anfang", plan.start_datum_);
	query.bindValue(":ende", plan.end_datum_);
	query.exec();
	int tp_id = query.lastInsertId().toInt();
	for (auto te : plan.trainingseinheiten_) {
		te->trainingsplan_id_ = tp_id;
		Trainingseinheit::AddTrainingseinheit(*te);
	}
	query.finish();
	return tp_id;
}

std::vector<Trainingsplan*> Trainingsplan::GetTrainingsplan(int user_id)
{
	std::vector<Trainingsplan*> plane;
	QString prep = "SELECT * FROM dbo.Trainingsplan WHERE BenutzerID = :user_id";
	QSqlQuery query;
	query.prepare(prep);
	query.bindValue(":user_id", user_id);
	query.exec();
	while (query.next()) {
		Trainingsplan* p = new Trainingsplan();
		p->id_ = query.value("TrainingsplanID").toInt();
		p->start_datum_ = query.value("Anfangsdatum").toString();
		p->end_datum_ = query.value("Enddatum").toString();
		p->trainingseinheiten_ = Trainingseinheit::GetTrainingseinheit(p->id_);
		plane.push_back(p);
	}
	query.finish();
	return plane;
}

std::vector<Trainingsplan*> Trainingsplan::GetTrainingsplan(int user_id, QString start_date)
{
	std::vector<Trainingsplan*> plane;
	QString prep = "SELECT * FROM dbo.Trainingsplan WHERE Enddatum >= :end AND BenutzerID = :user_id";
	QSqlQuery query;
	query.prepare(prep);
	query.bindValue(":end", start_date);
	query.bindValue(":user_id", user_id);
	query.exec();
	while (query.next()) {
		Trainingsplan* p = new Trainingsplan();
		p->id_ = query.value("TrainingsplanID").toInt();
		p->start_datum_ = query.value("Anfangsdatum").toString();
		p->end_datum_ = query.value("Enddatum").toString();
		p->trainingseinheiten_ = Trainingseinheit::GetTrainingseinheit(p->id_);
		plane.push_back(p);
	}
	query.finish();
	return plane;
}

std::vector<Trainingsplan*> Trainingsplan::GetTrainingsplan(int user_id, QString start_date, QString end_date)
{
	std::vector<Trainingsplan*> plane;
	QString prep = "SELECT * FROM dbo.Trainingsplan WHERE Anfangsdatum >= :start AND Enddatum <= :end AND BenutzerID = :user_id";
	QSqlQuery query;
	query.prepare(prep);
	query.bindValue(":start", start_date);
	query.bindValue(":end", end_date);
	query.bindValue(":user_id", user_id);
	query.exec();
	while (query.next()) {
		Trainingsplan* p = new Trainingsplan();
		p->id_ = query.value("TrainingsplanID").toInt();
		p->start_datum_ = query.value("Anfangsdatum").toString();
		p->end_datum_ = query.value("Enddatum").toString();
		p->trainingseinheiten_ = Trainingseinheit::GetTrainingseinheit(p->id_);
		plane.push_back(p);
	}
	query.finish();
	return plane;
}
