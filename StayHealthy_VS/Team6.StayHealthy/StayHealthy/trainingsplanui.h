#pragma once

#include <QWidget>
#include "trainingsplan.h"
#include "qstackedwidget.h"

class TrainingsplanUI : public QWidget
{
	Q_OBJECT

public:
	std::vector<Trainingsplan*> plane_;
	void SetUpUI(Trainingsplan&);

private slots:

	void Create();
	void Update();
	void Delete();
	void SavePlan();
	void Next();
	void Prev();
	void Done();


private:
	int user_id_;
	int index;

	QWidget* plan_widget_;
	QStackedWidget* stack_;

public:
	TrainingsplanUI(QWidget *parent);
	~TrainingsplanUI();
};
