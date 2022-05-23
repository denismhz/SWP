#include "speiseposition.h"
#include <QtSql>

Speiseposition::Speiseposition() {

}

Speiseposition::~Speiseposition()
{
}

std::vector<Speiseposition*> Speiseposition::GetSpeiseposition(int mahlzeit_id)
{
    std::vector<Speiseposition*> sps;
    QString prep = "select * from Speiseposition "
        "WHERE MahlzeitID = :m_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":m_id", mahlzeit_id);
    query.exec();
    while (query.next())
    {
        Speiseposition* ue = new Speiseposition();
        ue->beschreibung_ = query.value("Beschreibung").toString();
        ue->menge_ = query.value("Menge").toInt();
        ue->speise_id_ = query.value("SpeiseID").toInt();
        ue->mahlzeit_id_ = query.value("MahlzeitID").toInt();
        ue->id_ = query.value("SpeisepositionID").toInt();
        sps.push_back(ue);
    }
    query.finish();
    return sps;
}

int Speiseposition::AddSpeiseposition(Speiseposition& sp)
{
    QString prep =
        "INSERT INTO dbo.Speiseposition (MahlzeitID, SpeiseID, Menge, Beschreibung) VALUES (:mahlzeit, :speise_id, :menge, :beschreibung)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":mahlzeit", sp.mahlzeit_id_);
    query.bindValue(":speise_id", sp.speise_id_);
    query.bindValue(":menge", sp.menge_);
    query.bindValue(":beschreibung", sp.beschreibung_);
    query.exec();
    query.finish();
    return query.lastInsertId().toInt();
}
