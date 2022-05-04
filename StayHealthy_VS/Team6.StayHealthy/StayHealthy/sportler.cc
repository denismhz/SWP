#include "sportler.h"
#include "qstringlist.h"
#include <chrono>

using namespace date;
using namespace std::chrono;

Sportler::Sportler()
{
}

Sportler::~Sportler()
{
}

int Sportler::GrundUmsatzBerechnen(int gewicht, int groesse, int alter, QString geschlecht)
{
    float grundumsatz = 0.0;
    if (geschlecht == "m") {
        grundumsatz = 66.47 + (13.7 * gewicht) + (5 * groesse) - (6.8 * alter);
        return grundumsatz;
    }
    grundumsatz = 665.1 + (9.6 * gewicht) + (1.8 * groesse) - (4.78 * alter);
    return int(grundumsatz);
}
