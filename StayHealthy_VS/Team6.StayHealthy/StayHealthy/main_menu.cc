#include "main_menu.h"
#include "trainingsplan.h"
#include "profil_bearbeiten.h"
#include "qpushbutton.h"
#include "trainingseinheit.h"
#include "login.h"
#include "qlayout.h"
#include "trainingsplanui.h"


MainMenu::MainMenu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	teui = new TrainingseinheitUI(this);
	me = new MahlzeitEingebenUI(this, 0);
	ep = new ErnaehrungsplanUI(this);
	tein = new TrainingseinheitEingeben(this, 0);
	tp = new TrainingsplanUI(this);
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	QPushButton* back = new QPushButton("zuruck",this);

	ui.stackedWidget->addWidget(me);
	ui.stackedWidget->addWidget(teui);
	ui.stackedWidget->addWidget(teui);
	ui.stackedWidget->addWidget(ep);
	ui.stackedWidget->addWidget(tein);
	ui.stackedWidget->addWidget(tp);

	main_layout->addWidget(back);
	main_layout->addWidget(ui.stackedWidget);

	connect(ui.toPlaenePage, SIGNAL(clicked()), this, SLOT(ErstellePlanTest()));
	connect(ui.trainingseinheitenButton, SIGNAL(clicked()), this, SLOT(on_trainingseinheitenButton_clicked()));
	connect(ui.toMahlzeitenPage, SIGNAL(clicked()), this, SLOT(on_Mahlzeit_clicked()));
	connect(back, SIGNAL(clicked()), this, SLOT(back_clicked()));
	connect(ui.ernaehrungsplaen_button, SIGNAL(clicked()), this, SLOT(on_Ernaehrungsplane_clicked()));
	connect(ui.toTrainingseinheitenEingeben, SIGNAL(clicked()), this, SLOT(on_TrainingseinheitButton_clicked()));
	connect(ui.toTrainingsplan, SIGNAL(clicked()), this, SLOT(on_TrainingsplanButton_clicked()));
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

void MainMenu::on_TrainingseinheitButton_clicked()
{
	ui.stackedWidget->setCurrentWidget(tein);
}

void MainMenu::on_TrainingsplanButton_clicked()
{
	tp->plane_ = Trainingsplan::GetTrainingsplan(Login::GetInstance()->GetLoggedInUser()->GetId());
	if (tp->plane_.size() == 0) {
		qDebug() << "no ep";
		Trainingsplan* ee = new Trainingsplan();
		tp->SetUpUI(*ee);
		ui.stackedWidget->setCurrentWidget(tp);
		return;
	}
	tp->SetUpUI(*tp->plane_[0]);
	ui.stackedWidget->setCurrentWidget(tp);
}

void MainMenu::ErstellePlanTest() {
	
	QDialog *date_get = new QDialog(this);
	QVBoxLayout* dialog_layout = new QVBoxLayout(date_get);

	QDateEdit* date_edit = new QDateEdit(date_get);
	dialog_layout->addWidget(date_edit);

	QPushButton* save = new QPushButton("Save", date_get);
	dialog_layout->addWidget(save);

	connect(save, SIGNAL(clicked()), date_get, SLOT(accept()));

	date_edit->setDate(QDate::currentDate());

	QDate end = date_edit->date().addDays(7);

	save->deleteLater();
	date_edit->deleteLater();
	dialog_layout->deleteLater();
	date_get->deleteLater();
	if (date_get->exec() == 0) return;
	if (!Trainingsplan::GetTrainingsplan(Login::GetInstance()->GetLoggedInUser()->GetId(), date_edit->date().toString(Qt::ISODate)).empty()) {
		//in diesem zeitraum gibts schon einen plan
		QMessageBox no = QMessageBox(this);
		no.setWindowTitle("NONO");
		no.setText("Es existiert bereits ein Trainingsplan in diesem Zeitraum oder Datum liegt in der Vergangenheit");
		no.exec();
		return;
	}
	Trainingsplan::ErstelleTrainingsplan(date_edit->date().toString(Qt::ISODate), 
		Profil::GetProfil(Login::GetInstance()->GetLoggedInUser()->GetId())->prefaerenz_);
}

void MainMenu::on_toProfilePage_clicked()
{
	profilWindow = new ProfilBearbeiten();
	connect(profilWindow, SIGNAL(profileWindow_Closing()), this, SLOT(on_ProfileWindow_Closing()));	
	profilWindow->show();
}