#include "date_parser.h"
#include "qstringlist.h"
#include <strstream>

using namespace std::chrono;

namespace dateparser{

    QString ParseDate(year_month_day ymd)
    {
        std::stringstream s;
        auto ymd_ = ymd.day() / ymd.month() / ymd.year();
        s << ymd_;
        return QString::fromUtf8(s.str());
    }

    year_month_day datefromint(int a, int b, int c)
    {
        year_month_day d = day(a) / month(b) / year(c);
        return d;
    }

    year_month_day QStringToYearMonthDate(QString date_format) {
        int _day, _month, _year;
        QStringList data = date_format.split(QLatin1Char('-'));
        _year = data[0].toInt();
        _month = data[1].toInt();
        _day = data[2].toInt();
        year_month_day d = day(_day) / month(_month) / year(_year);
        return d;
    }

    QString GetCurrentWeekStartDate()
    {
        return QString();
    }

    QString GetCurrentWeekEndDate()
    {
        return QString();
    }
}
