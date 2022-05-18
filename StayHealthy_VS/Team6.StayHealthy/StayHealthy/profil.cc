#include "profil.h"
#include "qstringlist.h"
#include "qdatetime.h"

Profil::Profil()
{
    this->alter_ = AlterBerechnen(geb_datum_);
    this->grundumsatz_ = GrundumsatzBerechnen(weight_, height_, alter_, geschlecht_);
}

Profil::~Profil()
{
}

int Profil::GetGrundumsatz()
{
    return this->GrundumsatzBerechnen(weight_, height_, alter_, geschlecht_);
}

int Profil::GetAlter()
{
    return AlterBerechnen(geb_datum_);
}

//Datenbankfunktionen
bool Profil::AddProfil(Profil& profil, int user_id)
{
    QString prep =
        "INSERT INTO dbo.Profil (BenutzerID, Geschlecht, Vorname, "
        "Nachname, Praeferenz, GebDatum, Gewicht, Groeße, "
        "Grundumsatz, Kalorienaufnahme) VALUES (:user_id, :geschlecht, "
        ":vorname, :nachname, :pref, :geb, :gewicht, :groesse, :gu, :ka)";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", user_id);
    query.bindValue(":geschlecht", profil.geschlecht_);
    query.bindValue(":vorname", profil.vorname_);
    query.bindValue(":nachname", profil.name_);
    query.bindValue(":pref", profil.prefaerenz_);
    query.bindValue(":geb", profil.geb_datum_);
    query.bindValue(":gewicht", profil.weight_);
    query.bindValue(":groesse", profil.height_);
    query.bindValue(":gu", profil.grundumsatz_);
    query.bindValue(":ka", profil.kalorienaufnahme_);
    query.exec();
    query.finish();
    return 1;
}

Profil* Profil::GetProfil(int id)
{
    QString prep = "SELECT * FROM dbo.Profil WHERE BenutzerID = :user_id";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":user_id", id);
    query.exec();
    query.first();
    Profil* profil = new Profil();
    profil->geschlecht_ = query.value("Geschlecht").toInt();
    profil->vorname_ = query.value("Vorname").toString();
    profil->name_ = query.value("Nachname").toString();
    profil->prefaerenz_ = query.value("Praeferenz").toString();
    profil->geb_datum_ = query.value("GebDatum").toString();
    profil->weight_ = query.value("Gewicht").toInt();
    profil->height_ = query.value("Groesse").toInt();
    profil->grundumsatz_ = query.value("Grundumsatz").toInt();
    profil->kalorienaufnahme_ = query.value("Kalorienaufnahme").toInt();
    query.finish();
    return profil;
}

Profil* Profil::GetProfil(User& user)
{
    qDebug() << user.GetId();
    return GetProfil(user.GetId());
}

bool Profil::UpdateProfil(Profil& profil, int user_id)
{
    QString prep =
        "UPDATE dbo.Profil SET "
        "Nachname = :nachname, "
        "Vorname = :vorname, "
        "Groesse = :groesse, "
        "Grundumsatz = :grundumsatz, "
        "Kalorienaufnahme = :kalorienaufnahme, "
        "Geschlecht = :geschlecht, "
        "Gewicht = :gewicht, "
        "Praeferenz = :praeferenz, "
        "GebDatum = :gebdatum"
        " WHERE BenutzerID = :id;";


    QSqlQuery query;
    query.prepare(prep);

    query.bindValue(":nachname", profil.name_);
    query.bindValue(":vorname", profil.vorname_);
    query.bindValue(":groesse", profil.height_);
    query.bindValue(":grundumsatz", profil.GetGrundumsatz());
    query.bindValue(":kalorienaufnahme", profil.kalorienaufnahme_);
    query.bindValue(":geschlecht", profil.geschlecht_);
    query.bindValue(":gewicht", profil.weight_);
    query.bindValue(":praeferenz", profil.prefaerenz_);
    query.bindValue(":gebdatum", profil.geb_datum_);
    query.bindValue(":id", user_id);


    qDebug() << "emailDB: " << profil.name_;

    query.exec();
    query.finish();
    return 1;
}

int Profil::GrundumsatzBerechnen(int gewicht, int groesse, int alter, int geschlecht)
{
    float grundumsatz = 0.0;
    if (geschlecht == 1) {
        grundumsatz = 66.47 + (13.7 * gewicht) + (5 * groesse) - (6.8 * alter);
        return grundumsatz;
    }
    grundumsatz = 665.1 + (9.6 * gewicht) + (1.8 * groesse) - (4.78 * alter);
    return int(grundumsatz);
}

int Profil::AlterBerechnen(QString geb_datum)
{
    //das stimmt noch nicht ganz
    int alter = 0;
    if (geb_datum.isEmpty())
        return 0;
    
    QDate today = QDate::currentDate();
    QDate date = QDate::fromString(geb_datum, Qt::ISODate);
    alter = today.year() - date.year();
    if ((date.month() - today.month()) <= 0) {
        alter += 1;
    }
    return alter;
}
