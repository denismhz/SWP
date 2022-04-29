#pragma once

#include <QWidget>
#include "ui_tests_stacked.h"

class TestsStackedClass : public QWidget
{
	Q_OBJECT

public:
	TestsStackedClass(QWidget *parent = Q_NULLPTR);
	~TestsStackedClass();

public slots :
       void a_clicke();
       void b_clicke();

private:
	Ui::TestsStackedClass ui;
};
