#include "trainingsplanui.h"
#include "qlayout.h"
#include "login.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qscrollarea.h"
#include "qdatetime.h"
#include "qdatetimeedit.h"
#include "trainingsplan.h"
#include "qdialog.h"
#include "trainingseinheit_eingeben.h"

TrainingsplanUI::TrainingsplanUI(QWidget *parent)
    : QWidget(parent)
{
	index = 0;
	plan_widget_ = new QWidget(this);
	QHBoxLayout* main_layout = new QHBoxLayout(this);
}

TrainingsplanUI::~TrainingsplanUI()
{
}

void TrainingsplanUI::SetUpUI(Trainingsplan& plan)
{
	User* user = Login::GetInstance()->GetLoggedInUser();
	user_id_ = user->GetId();

	qDeleteAll(plan_widget_->children());
	QVBoxLayout* main_layout = new QVBoxLayout(plan_widget_);

	QWidget* title_widget = new QWidget(plan_widget_);
	QHBoxLayout* title_layout = new QHBoxLayout(title_widget);
	title_layout->setAlignment(Qt::AlignCenter);
	QString title_txt = QString("<h5>T-plan %0 / %1 von %2 bis %3</h5>")
		.arg(index + 1).arg(plane_.size()).arg(plan.start_datum_).arg(plan.end_datum_);
	QLabel* title = new QLabel(title_txt, title_widget);
	QPushButton* next = new QPushButton("next", title_widget);
	connect(next, SIGNAL(clicked()), this, SLOT(Next()));
	QPushButton* prev = new QPushButton("prev", title_widget);
	connect(prev, SIGNAL(clicked()), this, SLOT(Prev()));
	title_layout->addWidget(prev);
	title_layout->addWidget(title);
	title_layout->addWidget(next);

	main_layout->addWidget(title_widget);

	//add Mahlzeiten widget
	QStackedWidget* m_stack = new QStackedWidget(plan_widget_);
	m_stack->setObjectName("m_stack");
	QVBoxLayout* m_stack_layout = new QVBoxLayout(m_stack);
	m_stack_layout->setAlignment(Qt::AlignCenter);
	main_layout->addWidget(m_stack);

	for (int i = 0; i < plan.trainingseinheiten_.size(); i++) {
		auto m = plan.trainingseinheiten_[i];
		QWidget* m_widget = new QWidget(m_stack);
		QVBoxLayout* m_main_layout = new QVBoxLayout(m_widget);

		//qDebug() << m->id_ << ":id widget";
		QWidget* m_title_widget = new QWidget(m_widget);
		m_title_widget->setObjectName(QString::number(m->id_));
		QHBoxLayout* m_title_layout = new QHBoxLayout(m_title_widget);
		m_title_layout->setAlignment(Qt::AlignCenter);
		QString m_title_txt = QString("<h6>Trainingseinheit %0 / %1 am %2 um</h6>")
			.arg(i + 1).arg(plan.trainingseinheiten_.size()).arg(m->datum_).arg(m->datum_);
		QLabel* m_title = new QLabel(m_title_txt, m_title_widget);

		QPushButton* m_next = new QPushButton("next", m_title_widget);
		m_next->setObjectName("mnext");
		connect(m_next, SIGNAL(clicked()), this, SLOT(Next()));
		QPushButton* m_prev = new QPushButton("prev", m_title_widget);
		m_prev->setObjectName("mprev");
		connect(m_prev, SIGNAL(clicked()), this, SLOT(Prev()));

		QScrollArea* scroll_area = new QScrollArea(m_widget);
		scroll_area->setWidgetResizable(true);

		m_title_layout->addWidget(m_prev);
		m_title_layout->addWidget(m_title);
		m_title_layout->addWidget(m_next);
		m_main_layout->addWidget(m_title_widget);
		m_main_layout->addWidget(scroll_area);
		m_stack->addWidget(m_widget);

		//and for all of these setup all the uebungspos widgets
		qDebug() << m->uebungspositionen.size() << " :uebungspos size";
		QWidget* for_scroll = new QWidget(scroll_area);
		QVBoxLayout* for_scroll_layout = new QVBoxLayout(for_scroll);
		for_scroll_layout->setAlignment(Qt::AlignTop);
		for (auto sp : m->uebungspositionen) {
			QWidget* uebungspos_widget = new QWidget(this);
			uebungspos_widget->setObjectName("uebungspos_widget");
			for_scroll_layout->addWidget(uebungspos_widget);

			QHBoxLayout* uebungspos_layout = new QHBoxLayout(uebungspos_widget);
			uebungspos_layout->setAlignment(Qt::AlignTop);

			QLabel* uebungspos_beschreibung = new QLabel(sp->beschreibung_, uebungspos_widget);
			QLabel* uebungspos_menge = new QLabel(QString::number(sp->menge_), uebungspos_widget);
			uebungspos_layout->addWidget(uebungspos_beschreibung);
			uebungspos_layout->addWidget(uebungspos_menge);
		}
		scroll_area->setWidget(for_scroll);
	}

	QWidget* footer_widget = new QWidget(plan_widget_);
	QHBoxLayout* footer_layout = new QHBoxLayout(footer_widget);
	QPushButton* new_plan_button = new QPushButton("Neuer Plan", footer_widget);
	connect(new_plan_button, SIGNAL(clicked()), this, SLOT(Create()));
	footer_layout->addWidget(new_plan_button);
	footer_layout->setAlignment(Qt::AlignLeft);
	new_plan_button->setFixedWidth(150);
	QPushButton* delete_plan_button = new QPushButton("Plan löschen", footer_widget);
	connect(delete_plan_button, SIGNAL(clicked()), this, SLOT(Delete()));
	delete_plan_button->setFixedWidth(150);
	footer_layout->addWidget(delete_plan_button);
	main_layout->addWidget(footer_widget);


	m_stack->setCurrentIndex(0);
	this->layout()->addWidget(plan_widget_);
}

void TrainingsplanUI::Create() {
	qDeleteAll(plan_widget_->children());
	QVBoxLayout* main_layout = new QVBoxLayout(plan_widget_);
	//check whether theres no plan in that timeframe
	QDialog* date = new QDialog(plan_widget_);
	QVBoxLayout* dialog_layout = new QVBoxLayout(date);
	QDateEdit* start_date = new QDateEdit(QDate::currentDate(), plan_widget_);
	dialog_layout->addWidget(start_date);
	QPushButton* submit_plan_button = new QPushButton("Submit Plan", date);
	dialog_layout->addWidget(submit_plan_button);
	connect(submit_plan_button, SIGNAL(clicked()), this, SLOT(SavePlan()));
	date->exec();


	//add label here and maybe do new widget for form


	QString end = start_date->date().addDays(7).toString(Qt::ISODate);
	QString start = start_date->date().toString(Qt::ISODate);
	//plan in datenbank einfügen
	qDebug() << start;
	Trainingsplan* new_plan = new Trainingsplan();
	plane_.push_back(new_plan);
	new_plan->start_datum_ = start;
	new_plan->end_datum_ = end;
	new_plan->user_id_ = Login::GetInstance()->GetLoggedInUser()->GetId();
	new_plan->SetID(Trainingsplan::AddTrainingsplan(*plane_[plane_.size() - 1]));

	qDebug() << "new plan id" << new_plan->GetID();

	TrainingseinheitEingeben* meingabe = new TrainingseinheitEingeben(plan_widget_, new_plan->GetID());

	QPushButton* done = new QPushButton("Fertig", this);
	connect(done, SIGNAL(clicked()), this, SLOT(Done()));

	plan_widget_->layout()->addWidget(meingabe);
	plan_widget_->layout()->addWidget(done);
}

void TrainingsplanUI::Update()
{
}

void TrainingsplanUI::Delete()
{
	Trainingsplan::DeleteTrainingsplan(plane_[index]->GetID());
	for (auto m : plane_[index]->trainingseinheiten_) {
		Trainingseinheit::DeleteTrainingseinheit(m->id_);
	}
	//remove from plane
	std::vector<Trainingsplan*>::iterator it = plane_.begin() + index;
	qDebug() << (*it)->GetID() << " :erased";
	plane_.erase(it);
	if (plane_.size() <= 0) {
		Trainingsplan* ep = new Trainingsplan();
		SetUpUI(*ep);
		return;
	}
	index = 0;
	SetUpUI(*plane_[index]);
}

void TrainingsplanUI::SavePlan()
{
	plan_widget_->findChild<QDialog*>()->accept();
}

void TrainingsplanUI::Next()
{
	QStackedWidget* m_stack = this->findChild<QStackedWidget*>("m_stack");
	QObject* asd = sender();
	if (asd->objectName() == "mnext") {
		m_stack->setCurrentIndex(m_stack->currentIndex() + 1);
	}
	else {
		if (index + 1 >= plane_.size()) return;
		qDeleteAll(plan_widget_->children());
		SetUpUI(*plane_[++index]);
	}
}

void TrainingsplanUI::Prev()
{
	QStackedWidget* m_stack = this->findChild<QStackedWidget*>("m_stack");
	QObject* asd = sender();
	if (asd->objectName() == "mprev") {
		m_stack->setCurrentIndex(m_stack->currentIndex() - 1);
	}
	else {
		if (index - 1 < 0) return;
		qDeleteAll(plan_widget_->children());
		SetUpUI(*plane_[--index]);
	}
}

void TrainingsplanUI::Done()
{
	plane_ = Trainingsplan::GetTrainingsplan(user_id_);
	index = 0;
	SetUpUI(*plane_[index]);
}
