#pragma once

#include <QWidget>
#include "ui_main_menu.h"
#include "stayhealthy.h"

// Forward Declarations
class ProfilBearbeiten;	

class MainMenu : public QWidget
{
	Q_OBJECT

public:
	MainMenu(QWidget *parent = Q_NULLPTR);
	~MainMenu();

	ProfilBearbeiten* profilWindow;

public slots:
	void ErstellePlanTest();

private slots:
	void on_toProfilePage_clicked();
	void on_ProfileWindow_Closing();

private:
	Ui::MainMenu ui;


};
