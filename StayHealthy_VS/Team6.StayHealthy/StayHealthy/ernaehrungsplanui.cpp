#include "ernaehrungsplanui.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "db_handler.h"
#include "qlayout.h"
#include "login.h"
#include "qdatetime.h"
#include "qscrollarea.h"
#include "mahlzeit_eingeben_ui.h"
#include <qdatetimeedit.h>
#include "qdialog.h"

ErnaehrungsplanUI::ErnaehrungsplanUI(QWidget *parent)
	: QWidget(parent)
{
	index = 0;
	plan_widget_ = new QWidget(this);
	QHBoxLayout* main_layout = new QHBoxLayout(this);

	//get ernaehrungsplan for first setup

}

ErnaehrungsplanUI::~ErnaehrungsplanUI()
{
}

void ErnaehrungsplanUI::SetUpUI(Ernaehrungsplan& plan) {
	User* user = Login::GetInstance()->GetLoggedInUser();
	user_id_ = user->GetId();
	
	qDeleteAll(plan_widget_->children());
	QVBoxLayout* main_layout = new QVBoxLayout(plan_widget_);
	
	QWidget* title_widget = new QWidget(plan_widget_);
	QHBoxLayout* title_layout = new QHBoxLayout(title_widget);
	title_layout->setAlignment(Qt::AlignCenter);
	QString title_txt = QString("<h3>E-plan: %0 / %1 vom %2 bis %3<\h3>")
		.arg(index+1).arg(plane_.size()).arg(plan.start_datum_).arg(plan.end_dataum);
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

	for (auto m : plan.mahlzeiten) {
		QWidget* m_widget = new QWidget(m_stack);
		QVBoxLayout* m_main_layout = new QVBoxLayout(m_widget);

		//qDebug() << m->id_ << ":id widget";
		QWidget* m_title_widget = new QWidget(m_widget);
		m_title_widget->setObjectName(QString::number(m->id_));
		QHBoxLayout* m_title_layout = new QHBoxLayout(m_title_widget);
		m_title_layout->setAlignment(Qt::AlignCenter);
		QString m_title_txt = QString("<h4>Mahlzeit %0 / %1 am %2<\h4>")
			.arg(m->id_).arg(plan.mahlzeiten.size()).arg(m->datum_);
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

		//and for all of these setup all the speisepos widgets
		qDebug() << m->speisepositionen.size() << " :speisepos size";
		QWidget* for_scroll = new QWidget(scroll_area);
		QVBoxLayout* for_scroll_layout = new QVBoxLayout(for_scroll);
		for_scroll_layout->setAlignment(Qt::AlignTop);
		for (auto sp : m->speisepositionen) {
			QWidget* speisepos_widget = new QWidget(this);
			speisepos_widget->setObjectName("speisepos_widget");
			for_scroll_layout->addWidget(speisepos_widget);

			QHBoxLayout* speisepos_layout = new QHBoxLayout(speisepos_widget);
			speisepos_layout->setAlignment(Qt::AlignTop);

			QLabel* speisepos_beschreibung = new QLabel(sp->beschreibung_, speisepos_widget);
			QLabel* speisepos_menge = new QLabel(QString::number(sp->menge_), speisepos_widget);
			speisepos_layout->addWidget(speisepos_beschreibung);
			speisepos_layout->addWidget(speisepos_menge);
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


void ErnaehrungsplanUI::Create()
{
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
	Ernaehrungsplan* new_plan = new Ernaehrungsplan();
	plane_.push_back(new_plan);
	new_plan->start_datum_ = start;
	new_plan->end_dataum = end;
	new_plan->user_id_ = Login::GetInstance()->GetLoggedInUser()->GetId();
	new_plan->SetID(Ernaehrungsplan::AddErnaehrungsplan(*plane_[plane_.size() - 1]));

	MahlzeitEingebenUI* meingabe = new MahlzeitEingebenUI(plan_widget_, new_plan->GetID());

	QPushButton* done = new QPushButton("Fertig", this);
	connect(done, SIGNAL(clicked()), this, SLOT(Done()));

	plan_widget_->layout()->addWidget(meingabe);
	plan_widget_->layout()->addWidget(done);
}

void ErnaehrungsplanUI::Update()
{
}

void ErnaehrungsplanUI::Delete()
{
	Ernaehrungsplan::DeleteErnaehrungsplan(plane_[index]->GetID());
	//remove from plane
	std::vector<Ernaehrungsplan*>::iterator it = plane_.begin() + index;
	qDebug() << (*it)->GetID() << " :erased";
	plane_.erase(it);
	if (plane_.size() <= 0) {
		Ernaehrungsplan* ep = new Ernaehrungsplan();
		SetUpUI(*ep);
		return;
	}
	index = 0;
	SetUpUI(*plane_[index]);
}

void ErnaehrungsplanUI::SavePlan()
{
	plan_widget_->findChild<QDialog*>()->accept();
}

void ErnaehrungsplanUI::Next()
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

void ErnaehrungsplanUI::Prev()
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

void ErnaehrungsplanUI::Done()
{
	plane_ = Ernaehrungsplan::GetErnaehrungsplan(user_id_);
	index = 0;
	SetUpUI(*plane_[index]);
}
