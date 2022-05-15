#include "main_menu.h"
#include "trainingsplan.h"
#include "profil_bearbeiten.h"


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

void MainMenu::on_ProfileWindow_Closing()
{
	qDebug() << "onClosing";
	delete profilWindow;
	profilWindow = NULL;
}

void MainMenu::ErstellePlanTest() {
	Trainingsplan* n{}; n->ErstelleTrainingsplan("2022-06-03", "");
	delete n;
	//qDebug() << n->trainingseinheiten_[1]->uebungspositionen[1]->beschreibung_;
}

void MainMenu::on_toProfilePage_clicked()
{
	profilWindow = new ProfilBearbeiten();
	connect(profilWindow, SIGNAL(profileWindow_Closing()), this, SLOT(on_ProfileWindow_Closing()));	
	profilWindow->show();
}
