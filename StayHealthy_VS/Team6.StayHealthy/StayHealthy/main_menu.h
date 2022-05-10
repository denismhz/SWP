#pragma once

#include <QWidget>
#include "ui_main_menu.h"
#include "stayhealthy.h"

class MainMenu : public QWidget
{
	Q_OBJECT

public:
	MainMenu(QWidget *parent = Q_NULLPTR);
	~MainMenu();

private:
	Ui::MainMenu ui;
};
