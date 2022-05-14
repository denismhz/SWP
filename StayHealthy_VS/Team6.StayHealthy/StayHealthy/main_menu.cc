#include "main_menu.h"
#include "trainingsplan.h"
#include "trainingseinheitui.h"

MainMenu::MainMenu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	teui = new TrainingseinheitUI(this);
	ui.stackedWidget->addWidget(teui);
	ui.stackedWidget->layout()->addWidget(teui);
	connect(ui.homeButton, SIGNAL(clicked()), parentWidget(), SLOT(HomePressed())); 
	connect(ui.toPlaenePage, SIGNAL(clicked()), this, SLOT(ErstellePlanTest()));
	connect(ui.trainingseinheitenButton, SIGNAL(clicked()), this, SLOT(on_trainingseinheitenButton_clicked()));
}

MainMenu::~MainMenu()
{
}

void MainMenu::on_trainingseinheitenButton_clicked()
{
	teui->SetUp();
	ui.stackedWidget->setCurrentWidget(teui);
}

void MainMenu::ErstellePlanTest() {
	Trainingsplan* n{}; n->ErstelleTrainingsplan("2022-06-03", "");
	delete n;
	
	//qDebug() << n->trainingseinheiten_[1]->uebungspositionen[1]->beschreibung_;
}