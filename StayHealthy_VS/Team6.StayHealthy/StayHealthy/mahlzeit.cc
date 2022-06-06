#include "mahlzeit.h"
#include <QtSql>

Mahlzeit::Mahlzeit()
{
}

Mahlzeit::~Mahlzeit()
{
}

int Mahlzeit::GetKaloriengehalt()
{
    int sum = 0;
    for (auto sp : this->speisepositionen) {
        sum += sp->kaloriengehalt_;
    }
    return sum;
}

std::vector<Mahlzeit*> Mahlzeit::GetMahlzeit(int user_id)
{
    std::vector<Mahlzeit*> mahlzeiten;
    QString prep = "SELECT * FROM dbo.Mahlzeit WHERE BenutzerID = :user_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.exec();
    while (query.next()) {
        Mahlzeit* p = new Mahlzeit();
        p->id_ = query.value("MahlzeitID").toInt();
        p->ernaehrungsplan_id_ = query.value("ErnaehrungsplanID").toInt();
        p->datum_ = query.value("Datum").toString();
        p->uhrzeit_ = query.value("Uhrzeit").toString();
        mahlzeiten.push_back(p);
    }
    query.finish();
    return mahlzeiten;
}

std::vector<Mahlzeit*> Mahlzeit::GetMahlzeit(int user_id, int eplan_id)
{
    std::vector<Mahlzeit*> mahlzeiten;
    QString prep = "SELECT * FROM dbo.Mahlzeit WHERE BenutzerID = :user_id AND ErnaehrungsplanID = :e_id;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.bindValue(":e_id", eplan_id);
    query.exec();
    while (query.next()) {
        Mahlzeit* p = new Mahlzeit();
        p->id_ = query.value("MahlzeitID").toInt();
        p->ernaehrungsplan_id_ = query.value("ErnaehrungsplanID").toInt();
        p->datum_ = query.value("Datum").toString();
        p->uhrzeit_ = query.value("Uhrzeit").toString();
        qDebug() << p->id_ << " :id get";
        p->speisepositionen = Speiseposition::GetSpeiseposition(p->id_);
        mahlzeiten.push_back(p);
    }
    query.finish();
    return mahlzeiten;
}

int Mahlzeit::AddMahlzeit(Mahlzeit& m)
{
    QString prep =
        "INSERT INTO dbo.Mahlzeit (BenutzerID, ErnaehrungsplanID, Uhrzeit, Datum)"
        " VALUES(:user_id, :e_id, :uhrzeit, :datum)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", m.user_id_);
    query.bindValue(":e_id", m.ernaehrungsplan_id_);
    query.bindValue(":uhrzeit", m.uhrzeit_);
    query.bindValue(":datum", m.datum_);
    query.exec();
    int mahlzeit_id = query.lastInsertId().toInt();
    m.id_ = mahlzeit_id;
    query.finish();
    //then add all the speisepositions from this Mahlzeit
    for (auto sp : m.speisepositionen) {
        sp->mahlzeit_id_ = mahlzeit_id;
        Speiseposition::AddSpeiseposition(*sp);
    }
    return mahlzeit_id;
}

bool Mahlzeit::DeleteMahlzeit(int id)
{
    //delete Speisepos
    QString prep =
        "DELETE FROM dbo.Mahlzeit WHERE MahlzeitID = :m_id;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":m_id", id);
    if(!query.exec()) return false;
    query.finish();
    return true;
}
