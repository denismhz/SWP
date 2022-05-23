#pragma once

#include <QWidget>
#include "qstackedwidget.h"
#include "ernaerungsplan.h"

class ErnaehrungsplanUI : public QWidget
{
	Q_OBJECT

public:
	ErnaehrungsplanUI(QWidget *parent);
	~ErnaehrungsplanUI();

	std::vector<Ernaehrungsplan*> plane_;
	void SetUpUI(Ernaehrungsplan&);

private slots:

	void Create();
	void Update();
	void Delete();
	void SavePlan();
	void Next();
	void Prev();
	void Done();

signals:


private:
	int user_id_;
	int index;

	QWidget* plan_widget_;
	QStackedWidget* stack_;
};

//setup ui
