#pragma once

#include <QWidget>
#include "qlabel.h"
#include "qslider.h"
#include "qtableview.h"

class StatistikUI : public QWidget
{
	Q_OBJECT

private:
	QLabel* label_;
	QSlider* days_slider_;
	QTableView* tableView;
	int ep_insgesamt;
	int tp_insgesamt;

private slots:
	void DoSomething();

public:
	StatistikUI(QWidget *parent);
	~StatistikUI();
};
