#include "main_menu.h"
#include "trainingsplan.h"
#include "profil_bearbeiten.h"
#include "qpushbutton.h"
#include "trainingseinheit.h"
#include "login.h"


MainMenu::MainMenu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	teui = new TrainingseinheitUI(this);
	me = new MahlzeitEingebenUI(this, 0);
	ep = new ErnaehrungsplanUI(this);
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	QPushButton* back = new QPushButton("zuruck",this);

	ui.stackedWidget->addWidget(me);
	ui.stackedWidget->addWidget(teui);
	ui.stackedWidget->addWidget(teui);
	ui.stackedWidget->addWidget(ep);
	main_layout->addWidget(back);
	main_layout->addWidget(ui.stackedWidget);
	//connect(ui.homeButton, SIGNAL(clicked()), parentWidget(), SLOT(HomePressed())); 
	connect(ui.toPlaenePage, SIGNAL(clicked()), this, SLOT(ErstellePlanTest()));
	connect(ui.trainingseinheitenButton, SIGNAL(clicked()), this, SLOT(on_trainingseinheitenButton_clicked()));
	connect(ui.toMahlzeitenPage, SIGNAL(clicked()), this, SLOT(on_Mahlzeit_clicked()));
	connect(back, SIGNAL(clicked()), this, SLOT(back_clicked()));
	connect(ui.ernaehrungsplaen_button, SIGNAL(clicked()), this, SLOT(on_Ernaehrungsplane_clicked()));
	//ui.toPlaenePage->hide();
}

MainMenu::~MainMenu()
{
}

void MainMenu::on_trainingseinheitenButton_clicked()
{
	teui->SetUp();
	ui.stackedWidget->setCurrentWidget(teui);
}

void MainMenu::on_ProfileWindow_Closing()
{
	qDebug() << "onClosing";
	delete profilWindow;
	profilWindow = NULL;
}

void MainMenu::on_Mahlzeit_clicked()
{
	ui.stackedWidget->setCurrentWidget(me);
}

void MainMenu::on_Ernaehrungsplane_clicked()
{
	ep->plane_ = Ernaehrungsplan::GetErnaehrungsplan(Login::GetInstance()->GetLoggedInUser()->GetId());
	if (ep->plane_.size() == 0) {
		qDebug() << "no ep";
		Ernaehrungsplan* ee = new Ernaehrungsplan();
		ep->SetUpUI(*ee);
		ui.stackedWidget->setCurrentWidget(ep);
		return;
	}
	ep->SetUpUI(*ep->plane_[0]);
	ui.stackedWidget->setCurrentWidget(ep);
}

void MainMenu::back_clicked()
{
	ui.stackedWidget->setCurrentIndex(0);
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