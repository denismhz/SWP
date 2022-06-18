#include "ernaerungsplan.h"
#include "qdatetime.h"
#include <QtSql>
int Ernaehrungsplan::BerechneKaloriengehalt()
{
    return 0;
}

Ernaehrungsplan::Ernaehrungsplan()
{
}

Ernaehrungsplan::~Ernaehrungsplan()
{
    for (auto m : mahlzeiten) delete m;
}

int Ernaehrungsplan::GetID()
{
    return this->id_;
}

void Ernaehrungsplan::SetID(int id)
{
    this->id_ = id;
}

int Ernaehrungsplan::GetKaloriengehalt()
{
    return this->kaloriengehalt;
}

Ernaehrungsplan* Ernaehrungsplan::GetErnaehrungsplan(int user_id, QString today)
{
    Ernaehrungsplan* plan = new Ernaehrungsplan();
    QString prep = "SELECT * FROM dbo.Ernaehrungsplan WHERE Anfangsdatum <= :today AND Enddatum >= :today AND BenutzerID = :user_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":today", today);
    query.bindValue(":user_id", user_id);
    query.exec();
    query.first();
    plan->id_ = query.value("ErnaehrungsplanID").toInt();
    plan->start_datum_ = query.value("Anfangsdatum").toString();
    plan->end_dataum = query.value("Enddatum").toString();
    query.finish();
    plan->mahlzeiten = Mahlzeit::GetMahlzeit(user_id, plan->id_);
    return plan;
}

std::vector<Ernaehrungsplan*> Ernaehrungsplan::GetErnaehrungsplan(int user_id)
{
    std::vector<Ernaehrungsplan*> plane;
    
    QString prep = "SELECT * FROM dbo.Ernaehrungsplan WHERE BenutzerID = :user_id AND Anfangsdatum <= :today ORDER BY Anfangsdatum ASC";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.bindValue(":today", "2022-09-29");
    query.exec();
    while (query.next()) {
        Ernaehrungsplan* plan = new Ernaehrungsplan();
        plan->id_ = query.value("ErnaehrungsplanID").toInt();
        plan->start_datum_ = query.value("Anfangsdatum").toString();
        plan->end_dataum = query.value("Enddatum").toString();
        plan->mahlzeiten = Mahlzeit::GetMahlzeit(user_id, plan->id_);
        plane.push_back(plan);
    }
    query.finish();
    
    return plane;
}

int Ernaehrungsplan::AddErnaehrungsplan(Ernaehrungsplan& plan)
{
    QString prep =
        "INSERT INTO dbo.Ernaehrungsplan (BenutzerID, Anfangsdatum, Enddatum) VALUES (:user_id, :anfang, "
        ":ende)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", plan.user_id_);
    query.bindValue(":anfang", plan.start_datum_);
    query.bindValue(":ende", plan.end_dataum);
    query.exec();
    query.finish();
    int ep_id = query.lastInsertId().toInt();
    //add mahlzeiten
    for (auto m : plan.mahlzeiten) {
        m->ernaehrungsplan_id_ = ep_id;
        Mahlzeit::AddMahlzeit(*m);
    }
    return query.lastInsertId().toInt();
}

bool Ernaehrungsplan::DeleteErnaehrungsplan(int id)
{
    QString prep =
        "DELETE FROM dbo.Trainingsplan WHERE TrainingsplanID = :id;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":id", id);
    query.exec();    
    query.finish();
    return true;
}
