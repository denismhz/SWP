#include "uebungsposition.h"
#include <QtSql>

Uebungsposition::Uebungsposition()
{
}

Uebungsposition::Uebungsposition(int menge, int uebungs_id, int kalorienverbrauch, int trainingseinheit_id, QString beschreibung)
	: menge_(menge), uebungs_id_(uebungs_id), kalorienverbrauch_(kalorienverbrauch), trainingseinheit_id_(trainingseinheit_id), beschreibung_(beschreibung)
{
}

Uebungsposition::~Uebungsposition()
{
}

int Uebungsposition::KalorienverbrauchBerechnen()
{
	return 0;
}

int Uebungsposition::AddUebungsposition(Uebungsposition& uebungspos)
{
    QString prep =
        "INSERT INTO dbo.Uebungsposition (TrainingseinheitID, UebungsID, Menge, Beschreibung) VALUES (:te_id, :ue_id, :menge, "
        ":beschreibung)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":te_id", uebungspos.trainingseinheit_id_);
    query.bindValue(":ue_id", uebungspos.uebungs_id_);
    query.bindValue(":menge", uebungspos.menge_);
    query.bindValue(":beschreibung", uebungspos.beschreibung_);
    query.exec();
    query.finish();
    return query.lastInsertId().toInt();
}

bool Uebungsposition::UpdateUebungsposition(Uebungsposition& pos)
{
    QString prep = "Update Uebungsposition SET Menge = :menge, Beschreibung = :beschr WHERE UebungspositionID = :uebungs_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":uebungs_id", pos.uebungs_id_);
    query.bindValue(":menge", pos.menge_);
    query.bindValue(":beschr", pos.beschreibung_);
    query.exec();
    query.finish();
    return false;
}

std::vector<Uebungsposition*> Uebungsposition::GetUebungsposition(int trainingseinheit_id)
{
    std::vector<Uebungsposition*> ueps;
    QString prep = "select * from Uebungsposition "
        "WHERE TrainingseinheitID = :trainingseinheit_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":trainingseinheit_id", trainingseinheit_id);
    query.exec();
    while (query.next())
    {
        Uebungsposition* ue = new Uebungsposition();
        ue->beschreibung_ = query.value("Beschreibung").toString();
        ue->menge_ = query.value("Menge").toInt();
        ue->uebungs_id_ = query.value("UebungsID").toInt();
        ue->trainingseinheit_id_ = query.value("TrainingseinheitID").toInt();
        ue->id_ = query.value("UebungspositionID").toInt();
        ueps.push_back(ue);
    }
    query.finish();
    return ueps;
}
