#pragma once

#include <QWidget>
#include "trainingsplan.h"

class TrainingsplanUI : public QWidget
{
	Q_OBJECT

public:
	TrainingsplanUI(QWidget *parent);
	~TrainingsplanUI();

private:
	std::vector<Trainingsplan*> plane_;

private slots:
	void Next();
	void Prev();
};
