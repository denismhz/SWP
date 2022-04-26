#pragma once

#include <QWidget>
#include "ui_test_class_1.h"

class TestClass1 : public QWidget
{
	Q_OBJECT

public:
	TestClass1(QWidget *parent = Q_NULLPTR);
	~TestClass1();

private:
	Ui::TestClass1 ui;
};
