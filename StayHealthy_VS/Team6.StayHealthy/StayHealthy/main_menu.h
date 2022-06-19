#pragma once

#include <QWidget>
#include "ui_main_menu.h"
#include "stayhealthy.h"
#include "qstackedwidget.h"
#include "trainingseinheitui.h"
#include "mahlzeit_eingeben_ui.h"
#include "ernaehrungsplanui.h"
#include "trainingseinheit_eingeben.h"
#include "trainingsplanui.h"
#include "statistikui.h"

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
	void on_trainingseinheitenButton_clicked();

private slots:
	void on_toProfilePage_clicked();
	void on_ProfileWindow_Closing();
	void on_Mahlzeit_clicked();
	void on_Ernaehrungsplane_clicked();
	void back_clicked();
	void on_TrainingseinheitButton_clicked();
	void on_TrainingsplanButton_clicked();
	void on_StatistikButton_clicked();

private:
	Ui::MainMenu ui;
	TrainingseinheitUI* teui;
	MahlzeitEingebenUI* me;
	ErnaehrungsplanUI* ep;
	TrainingseinheitEingeben* tein;
	TrainingsplanUI* tp;
	StatistikUI* stat;
};
