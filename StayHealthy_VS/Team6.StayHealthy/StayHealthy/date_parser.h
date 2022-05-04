#pragma once
#include "date.h"
#include "qstring.h"

using namespace date;

namespace dateparser{
	QString ParseDate(year_month_day ymd);
	year_month_day datefromint(int, int, int);
	year_month_day QStringToYearMonthDate(QString date_format);
	QString GetCurrentWeekStartDate();
	QString GetCurrentWeekEndDate();
};

