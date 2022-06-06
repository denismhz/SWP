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
        ue->kaloriengehalt_ = query.value("Kaloriengehalt").toInt();
        sps.push_back(ue);
    }
    query.finish();
    return sps;
}

int Speiseposition::AddSpeiseposition(Speiseposition& sp)
{
    int kaloriengehalt = 0;
    QString prep =
        "INSERT INTO dbo.Speiseposition (MahlzeitID, SpeiseID, Menge, Beschreibung, Kaloriengehalt) "
        "VALUES (:mahlzeit, :speise_id, :menge, :beschreibung, :kalorien)";
    QString prep_2 = "SELECT * FROM dbo.Speise WHERE SpeiseID = :speise_id";
    
    QSqlQuery query;
    query.prepare(prep_2);
    query.bindValue(":speise_id", sp.speise_id_);
    query.exec();
    query.first();
    kaloriengehalt = float(query.value("Kaloriengehalt").toFloat() / query.value("Menge").toFloat()) * float(sp.menge_);
    query.finish();

    query.prepare(prep);
    query.bindValue(":mahlzeit", sp.mahlzeit_id_);
    query.bindValue(":speise_id", sp.speise_id_);
    query.bindValue(":menge", sp.menge_);
    query.bindValue(":beschreibung", sp.beschreibung_);
    query.bindValue(":kalorien", kaloriengehalt);
    query.exec();
    query.finish();
    return query.lastInsertId().toInt();
}

int Speiseposition::DeleteSpeiseposition(int id)
{
    QString prep =
        "DELETE FROM dbo.Speiseposition WHERE SpeisepositionID = :sp_id;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":sp_id", id);
    query.exec();
    query.finish();
    return true;
}
