#include "trainingsplan_ui.h"
#include "login.h"
#include "qpushbutton.h"
#include "qlayout.h"
#include "qlabel.h"
#include "qscrollarea.h"
#include "qstackedwidget.h"

TrainingsplanUI::TrainingsplanUI(QWidget *parent)
    : QWidget(parent)
{
	//main widget layout
	QVBoxLayout* main_layout = new QVBoxLayout(this);

    //Get all the Trainingsplane to show to user this might take a while need a logged in user first lol or can we call this later
    plane_ = Trainingsplan::GetTrainingsplan(Login::GetInstance()->GetLoggedInUser()->GetId());

	QStackedWidget* plane_stack = new QStackedWidget(this);

	main_layout->addWidget(plane_stack);
    
    //setup the ui
	for (int i = 0; i < plane_.size(); i++) {
		QWidget* trainingsplan_widget = new QWidget(this);
		User* user = Login::GetInstance()->GetLoggedInUser();

		qDeleteAll(trainingsplan_widget->children());
		QVBoxLayout* main_layout = new QVBoxLayout(trainingsplan_widget);

		QWidget* title_widget = new QWidget(trainingsplan_widget);
		QHBoxLayout* title_layout = new QHBoxLayout(title_widget);
		title_layout->setAlignment(Qt::AlignCenter);
		QString title_txt = QString("<h5>E-plan %0 / %1 von %2 bis %3</h5>")
			.arg(i + 1).arg(plane_.size()).arg(plane_[i]->start_datum_).arg(plane_[i]->end_datum_);
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
		QStackedWidget* m_stack = new QStackedWidget(trainingsplan_widget);
		m_stack->setObjectName("m_stack");
		QVBoxLayout* m_stack_layout = new QVBoxLayout(m_stack);
		m_stack_layout->setAlignment(Qt::AlignCenter);
		main_layout->addWidget(m_stack);

		for (int j = 0; j < plane_[i]->trainingseinheiten_.size(); j++) {
			auto m = plane_[i]->trainingseinheiten_[j];
			QWidget* m_widget = new QWidget(m_stack);
			QVBoxLayout* m_main_layout = new QVBoxLayout(m_widget);

			//qDebug() << m->id_ << ":id widget";
			QWidget* m_title_widget = new QWidget(m_widget);
			m_title_widget->setObjectName(QString::number(m->id_));
			QHBoxLayout* m_title_layout = new QHBoxLayout(m_title_widget);
			m_title_layout->setAlignment(Qt::AlignCenter);
			QString m_title_txt = QString("<h6>Mahlzeit %0 / %1 am %2 um</h6>")
				.arg(i + 1).arg(plane_[i]->trainingseinheiten_.size()).arg(m->datum_).arg(m->durchgefuehrt);
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
			//qDebug() << m->speisepositionen.size() << " :speisepos size";
			QWidget* for_scroll = new QWidget(scroll_area);
			QVBoxLayout* for_scroll_layout = new QVBoxLayout(for_scroll);
			for_scroll_layout->setAlignment(Qt::AlignTop);
			for (auto ue : m->uebungspositionen) {
				QWidget* speisepos_widget = new QWidget(this);
				speisepos_widget->setObjectName("speisepos_widget");
				for_scroll_layout->addWidget(speisepos_widget);

				QHBoxLayout* speisepos_layout = new QHBoxLayout(speisepos_widget);
				speisepos_layout->setAlignment(Qt::AlignTop);

				QLabel* speisepos_beschreibung = new QLabel(ue->beschreibung_, speisepos_widget);
				QLabel* speisepos_menge = new QLabel(QString::number(ue->menge_), speisepos_widget);
				speisepos_layout->addWidget(speisepos_beschreibung);
				speisepos_layout->addWidget(speisepos_menge);
			}
			scroll_area->setWidget(for_scroll);
		}

		/*QWidget* footer_widget = new QWidget(trainingsplan_widget);
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
		main_layout->addWidget(footer_widget);*/


		m_stack->setCurrentIndex(0);
		plane_stack->addWidget(trainingsplan_widget);
	}
}

TrainingsplanUI::~TrainingsplanUI()
{
}

void TrainingsplanUI::Next() {
	this->layout()->findChild<QStackedWidget*>();
}

void TrainingsplanUI::Prev() {

}
