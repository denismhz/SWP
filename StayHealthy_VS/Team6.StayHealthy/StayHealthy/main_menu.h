#pragma once

#include <QWidget>
#include "ui_main_menu.h"
#include "stayhealthy.h"
#include "qstackedwidget.h"
#include "trainingseinheitui.h"

class MainMenu : public QWidget
{
	Q_OBJECT

public:
	MainMenu(QWidget *parent = Q_NULLPTR);
	~MainMenu();

public slots:
	void ErstellePlanTest();
	void on_trainingseinheitenButton_clicked();

private:
	Ui::MainMenu ui;
	TrainingseinheitUI* teui;
};
