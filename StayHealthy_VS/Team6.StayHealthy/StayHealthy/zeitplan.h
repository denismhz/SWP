#pragma once
#include <qstring.h>
#include <vector>
#include "ernaerungsplan.h";
#include "trainingsplan.h";

class Zeitplan
{
	//Sammlung aller pläne der derzeitigen woche bzw aller einheiten/mahlzteiten die enthalten sind werden in kalender angezeigt
public:
	QString start_datum_;
	QString end_datum_;
	QString beschreibung_;
	std::vector<Ernaerungsplan> ernaerungsplaene;
	std::vector<Trainingsplan> trainingsplaene;
};

