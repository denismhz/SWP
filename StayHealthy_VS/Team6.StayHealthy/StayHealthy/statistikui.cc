#include "statistikui.h"
#include "qlayout.h"
#include "qdatetime.h"
#include "ernaerungsplan.h"
#include "trainingsplan.h"
#include "qstandarditemmodel.h"
#include "login.h"
#include "qtableview.h"
#include <QtSql>

StatistikUI::StatistikUI(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_layout = new QHBoxLayout(this);

	days_slider_ = new QSlider(this);
	days_slider_->singleStep();
	days_slider_->setTickInterval(1);
	days_slider_->setMaximum(30);
	label_ = new QLabel(days_slider_);
	label_->setObjectName("slider_label");
	label_->setText(QString::number(days_slider_->value()) + " Tage");

	main_layout->addWidget(days_slider_);
	main_layout->addWidget(label_);

	connect(days_slider_, SIGNAL(valueChanged(int)), this, SLOT(DoSomething()));
}

StatistikUI::~StatistikUI()
{
	for (auto it : this->children()) it->deleteLater();
}

void StatistikUI::DoSomething() {

	this->findChild<QWidget*>("wid")->deleteLater();
	QWidget* wid = new QWidget(this);
	wid->setObjectName("wid");
	tableView = new QTableView(wid);
	
	QVBoxLayout* wid_layout = new QVBoxLayout(wid);
	this->layout()->addWidget(wid);


	User* curr_user = Login::GetInstance()->GetLoggedInUser();
	//create table for multiple days
	label_->setText(QString::number(days_slider_->value()) + " Tage");
	QDate today = QDate::currentDate();
	QDate end_datum = today;
	QDate start_datum = end_datum.addDays(-days_slider_->value());

	std::vector<Mahlzeit*> m;
	std::vector<Trainingseinheit*> te;

	//das ist sehr ineffizient bessere lsung bentigt
	//z.b. vector member variable und nur jeweils ein tag pushen d.h. datenbank muss nur einen eintrag get keine ahnung ob das viel 
	//effizienter ist
	//und pop nicht vergessen wenn tage verringert werden

	QString prep = "Select COUNT(*) FROM dbo.Ernaehrungsplan";
	QSqlQuery query;
	query.prepare(prep);
	query.exec();
	query.first();
	int mahlzeiten_insgesamt = query.value(0).toInt();
	query.finish();

	prep = "Select COUNT(*) FROM dbo.Trainingsplan";
	query.prepare(prep);
	query.exec();
	query.first();
	int tp_insgesamt = query.value(0).toInt();
	query.finish();

	m = Mahlzeit::GetMahlzeit(curr_user->GetId(), start_datum.toString(Qt::ISODate));
	//ep = Ernaehrungsplan::GetErnaehrungsplan(curr_user->GetId(), start_datum.toString(Qt::ISODate));
	te = Trainingseinheit::GetTrainingseinheit(curr_user->GetId(), start_datum.toString(Qt::ISODate));

	//hier muss man noch beachten ob mahlzeiten auch wirklich genommen wurden und ob te durchgefuehrt wurden
	//wenn eine mahlzeit vorhanden ist dann wurde sie auch genommen
	//das gilt genauso für ernaehrungsplaene und Trainingsplaene

	int m_kalorien = 0;
	int durch = 0;
	for (auto m : m) {
		m_kalorien += m->GetKaloriengehalt();
		qDebug() << "kalor" << m_kalorien << "  " << m->speisepositionen.size();
	}
	int te_kalorien = 0;
	for (auto te : te) {
		te_kalorien += te->kalorienverbrauch;
		if (te->durchgefuehrt) {
			durch++;
		}
	}

	QLabel* ernaehrungsplaene = new QLabel(wid);
	QLabel* trainingsplaene = new QLabel(wid);

	ernaehrungsplaene->setText(QString("Durchgefuehrte Ernaehrungsplaene insgesamt: %0").arg(mahlzeiten_insgesamt));
	trainingsplaene->setText(QString("Durchgefuehrte Trainingsplaene insgesamt: %0").arg(tp_insgesamt));

	wid->layout()->addWidget(ernaehrungsplaene);
	wid->layout()->addWidget(trainingsplaene);

	//man braucht bei mahlzeit auch durchgefuehrt wegen statistik

	QStringList h_labels;
	QStringList v_labels;
	v_labels.push_back("Trainingseinheiten");
	v_labels.push_back("Mahlzeiten");
	h_labels.push_back("Kalorien/Tag");
	h_labels.push_back("Abgeschlossen");

	QStandardItemModel* model = new QStandardItemModel(2, 2);
	if (days_slider_->value() > 0) {
		QStandardItem* m_item = new QStandardItem(QString("%0").arg(te_kalorien / days_slider_->value()));
		QStandardItem* te_item = new QStandardItem(QString("%0").arg(m_kalorien / days_slider_->value()));

		QStandardItem* te_a = new QStandardItem(QString("%0 / %1").arg(durch).arg(te.size()));
		model->setItem(0, 0, m_item);
		model->setItem(1, 0, te_item);
		model->setItem(0, 1, te_a);
	}

	model->setHorizontalHeaderLabels(h_labels);
	model->setVerticalHeaderLabels(v_labels);

	tableView->setModel(model);
	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();

	wid->layout()->addWidget(tableView);

	//speicher freigeben
	for (auto m : m) {
		delete m;
	}
	for (auto te : te) {
		delete te;
	}
}

