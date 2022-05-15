#include "sportler.h"
#include "qstringlist.h"
#include <chrono>

using namespace date;
using namespace std::chrono;

Sportler::Sportler(int geschlecht, int weight, int height, int kalorienaufnahme, QString geb_datum, QString prefaerenz) : weight_(weight),
height_(height), geb_datum_(geb_datum), prefaerenz_(prefaerenz), kalorienaufnahme_(kalorienaufnahme), geschlecht_(geschlecht)
{
    this->alter_ = AlterBerechnen(geb_datum_);
    this->grundumsatz_ = GrundumsatzBerechnen(weight_, height_, alter_, geschlecht_);
}

Sportler::~Sportler()
{
    delete zeitplan_;
}

int Sportler::GetGrundumsatz()
{
    return this->GrundumsatzBerechnen(weight_, height_, alter_, geschlecht_);
}

int Sportler::GetAlter()
{
    return AlterBerechnen(geb_datum_);
}

int Sportler::GrundumsatzBerechnen(int gewicht, int groesse, int alter, int geschlecht)
{
    float grundumsatz = 0.0;
    if (geschlecht == 1) {
        grundumsatz = 66.47 + (13.7 * gewicht) + (5 * groesse) - (6.8 * alter);
        return grundumsatz;
    }
    grundumsatz = 665.1 + (9.6 * gewicht) + (1.8 * groesse) - (4.78 * alter);
    return int(grundumsatz);
}

int Sportler::AlterBerechnen(QString geb_datum)
{
    if (geb_datum.isEmpty())
        return 0;
    year_month_day today = sys_days{floor<days>(system_clock::now())};
    auto ymd_geb_datum = dateparser::QStringToYearMonthDate(geb_datum);
    auto alter = today.year() - ymd_geb_datum.year();
    return alter.count();
}
