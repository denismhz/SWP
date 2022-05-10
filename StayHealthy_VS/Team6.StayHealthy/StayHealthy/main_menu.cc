#include "main_menu.h"
#include "trainingsplan.h"

MainMenu::MainMenu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.homeButton, SIGNAL(clicked()), parentWidget(), SLOT(HomePressed())); 
	connect(ui.toPlaenePage, SIGNAL(clicked()), this, SLOT(ErstellePlanTest()));
}

MainMenu::~MainMenu()
{
}

void MainMenu::ErstellePlanTest() {
	Trainingsplan* n{}; n->ErstelleTrainingsplan("2022-06-03", "");
	delete n;
	//qDebug() << n->trainingseinheiten_[1]->uebungspositionen[1]->beschreibung_;
}