#include "trainingseinheit.h"
#include <QtSql>

Trainingseinheit::Trainingseinheit()
{
}

Trainingseinheit::Trainingseinheit(int id, int user_id, int calories, bool durch, QString start, QString ende, QString datum)
	: user_id_(user_id), kalorienverbrauch(calories), durchgefuehrt(durch), start_zeit_(start), end_zeit_(ende), datum_(datum), id_(id)
{
}

Trainingseinheit::~Trainingseinheit()
{
}

int Trainingseinheit::AddTrainingseinheit(Trainingseinheit& tein)
{
    QString prep =
        "INSERT INTO dbo.Trainingseinheit (BenutzerID, TrainingsplanID, Startzeit, Endzeit, Datum, Durchgefuehrt, Kalorienverbrauch)"
        " VALUES(:user_id, :t_id, :start, :ende, :datum, :durchgefuehrt, :kalorienverbrauch)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", tein.user_id_);
    query.bindValue(":t_id", tein.trainingsplan_id_);
    query.bindValue(":start", tein.start_zeit_);
    query.bindValue(":ende", tein.end_zeit_);
    query.bindValue(":datum", tein.datum_);
    query.bindValue(":durchgefuehrt", tein.durchgefuehrt);
    query.bindValue(":kalorienverbrauch", tein.kalorienverbrauch);
    query.exec();
    int te_id = query.lastInsertId().toInt();
    for (auto up : tein.uebungspositionen) {
        up->trainingseinheit_id_ = te_id;
        Uebungsposition::AddUebungsposition(*up);
    }
    query.finish();
    return te_id;
}

std::vector<Trainingseinheit*> Trainingseinheit::GetTrainingseinheit(int user_id)
{
    std::vector<Trainingseinheit*>tein;
    QString prep = "select * from Trainingseinheit "
        "WHERE BenutzerID = :user_id AND Durchgefuehrt = 0";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.exec();
    while (query.next())
    {
        Trainingseinheit* te = new Trainingseinheit();
        te->datum_ = query.value("Datum").toString();
        te->start_zeit_ = query.value("Startzeit").toString();
        te->end_zeit_ = query.value("Endzeit").toString();
        te->durchgefuehrt = query.value("Durchgefuehrt").toBool();
        te->kalorienverbrauch = query.value("Kalorienverbrauch").toInt();
        te->id_ = query.value("TrainingseinheitID").toInt();
        te->uebungspositionen = Uebungsposition::GetUebungsposition(te->id_);
        tein.push_back(te);
    }
    query.finish();
    //get uebungspositionen
    return tein;
}

std::vector<Trainingseinheit*> Trainingseinheit::GetTrainingseinheit(int user_id, int trainingsplan_id)
{
    std::vector<Trainingseinheit*>tein;
    QString prep = "select * from Trainingseinheit "
        "WHERE BenutzerID = :user_id AND TrainingsplanID = :tp_id AND Durchgefuehrt = 0";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.bindValue(":tp_id", trainingsplan_id);
    query.exec();
    while (query.next())
    {
        Trainingseinheit* te = new Trainingseinheit();
        te->datum_ = query.value("Datum").toString();
        te->start_zeit_ = query.value("Startzeit").toString();
        te->end_zeit_ = query.value("Endzeit").toString();
        te->durchgefuehrt = query.value("Durchgefuehrt").toBool();
        te->kalorienverbrauch = query.value("Kalorienverbrauch").toInt();
        te->id_ = query.value("TrainingseinheitID").toInt();
        te->uebungspositionen = Uebungsposition::GetUebungsposition(te->id_);
        tein.push_back(te);
    }
    query.finish();
    return tein;
}

bool Trainingseinheit::DeleteTrainingseinheit(int id)
{
    QString prep =
        "DELETE FROM dbo.Trainingseinheit WHERE TrainingseinheitID = :m_id;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":m_id", id);
    if (!query.exec()) return false;
    query.finish();
    return true;
}
