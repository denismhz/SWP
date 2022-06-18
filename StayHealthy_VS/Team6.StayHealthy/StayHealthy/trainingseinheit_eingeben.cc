#include "trainingseinheit_eingeben.h"
#include "qlayout.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "qlineedit.h"
#include <QtSql>
#include "qmessagebox.h"
#include "qdatetimeedit.h"
#include "login.h"

TrainingseinheitEingeben::TrainingseinheitEingeben(QWidget *parent, int tp_id)
    : QWidget(parent)
{
    this->tp_id = tp_id;
    uebungspos_area = new QScrollArea(this);
    new_trainingseinheit_ = nullptr;
    scroll_widget_ = new QWidget(this);
    QVBoxLayout* scroll_layout = new QVBoxLayout(scroll_widget_);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignTop);

    QPushButton* add_new_trainingseinheit = new QPushButton("Neue Trainingseinheit erstellen", this);
    add_new_trainingseinheit->setObjectName("add_new_trainingseinheit");
    connect(add_new_trainingseinheit, SIGNAL(clicked()), this, SLOT(ShowEingabe()));
    main_layout->addWidget(add_new_trainingseinheit);

    QWidget* trainingseinheit_eingeben_widget = new QWidget(this);
    trainingseinheit_eingeben_widget->setObjectName("trainingseinheit_eingeben_widget");
    main_layout->addWidget(trainingseinheit_eingeben_widget);
    trainingseinheit_eingeben_widget->hide();

    main_layout->addWidget(uebungspos_area);
    uebungspos_area->setWidget(this->findChild<QWidget*>("uebungspos_widget"));

    QHBoxLayout* eingabe_layout = new QHBoxLayout(trainingseinheit_eingeben_widget);


    QDateEdit* date_edit = new QDateEdit(trainingseinheit_eingeben_widget);
    date_edit->setObjectName("date_edit");
    date_edit->setDate(QDate::currentDate());
    main_layout->addWidget(date_edit);

    QComboBox* uebungen = new QComboBox(trainingseinheit_eingeben_widget);
    uebungen->setObjectName("uebungen");
    eingabe_layout->addWidget(uebungen);
    FillComboBox(uebungen);

    QLineEdit* menge = new QLineEdit("10", trainingseinheit_eingeben_widget);
    QLabel* wdh = new QLabel("Wdh", trainingseinheit_eingeben_widget);
    menge->setObjectName("menge");
    menge->setAlignment(Qt::AlignRight);
    menge->setFixedWidth(160);
    eingabe_layout->addWidget(menge);
    eingabe_layout->addWidget(wdh);

    QPushButton* add_uebung_button = new QPushButton("Hinzufuegen", trainingseinheit_eingeben_widget);
    connect(add_uebung_button, SIGNAL(clicked()), this, SLOT(AddUebungsPos()));
    add_uebung_button->setFixedWidth(100);
    eingabe_layout->addWidget(add_uebung_button);

    QPushButton* done = new QPushButton("Trainingseinheit hinzufügen", this);
    connect(done, SIGNAL(clicked()), this, SLOT(Finish()));
    main_layout->addWidget(done);
}

TrainingseinheitEingeben::~TrainingseinheitEingeben()
{
}

void TrainingseinheitEingeben::ShowEingabe() {
    this->findChild<QWidget*>("add_new_trainingseinheit")->hide();
    this->findChild<QWidget*>("trainingseinheit_eingeben_widget")->show();
    new_trainingseinheit_ = new Trainingseinheit();
    new_trainingseinheit_->kalorienverbrauch = 0;
}

void TrainingseinheitEingeben::AddUebungsPos()
{
    Uebungsposition* uebungspos = new Uebungsposition();
    uebungspos->menge_ = this->findChild<QLineEdit*>("menge")->text().toInt();
    uebungspos->beschreibung_ = this->findChild<QComboBox*>("uebungen")->currentText();

    //get uebungsid
    QString prep = "SELECT UebungsID, Kalorienverbrauch from Uebung WHERE Titel = :titel;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":titel", uebungspos->beschreibung_);
    query.exec();
    query.first();
    uebungspos->uebungs_id_ = query.value("UebungsID").toInt();
    new_trainingseinheit_->kalorienverbrauch += query.value("Kalorienverbrauch").toInt() * uebungspos->menge_;

    QWidget* uebungspos_widget = new QWidget(scroll_widget_);
    uebungspos_widget->setObjectName("uebungspos_widget");
    scroll_widget_->layout()->addWidget(uebungspos_widget);

    QHBoxLayout* uebungspos_layout = new QHBoxLayout(uebungspos_widget);
    new_trainingseinheit_->uebungspositionen.push_back(uebungspos);
    qDebug() << new_trainingseinheit_->uebungspositionen.size();

    QLabel* uebungspos_beschreibung = new QLabel(uebungspos->beschreibung_, uebungspos_widget);
    QLabel* uebungspos_menge = new QLabel(QString::number(uebungspos->menge_), uebungspos_widget);
    uebungspos_layout->addWidget(uebungspos_beschreibung);
    uebungspos_layout->addWidget(uebungspos_menge);
    uebungspos_area->setWidget(scroll_widget_);
    uebungspos_area->setWidgetResizable(true);
}

void TrainingseinheitEingeben::Finish()
{
    if (!new_trainingseinheit_) {
        QMessageBox* no = new QMessageBox(this);
        no->setWindowTitle("alert");
        no->setText("Bitte Trainingseinheit erstellen");
        no->exec();
        return;
    }
    else if (new_trainingseinheit_->uebungspositionen.size() <= 0) {
        QMessageBox* no = new QMessageBox(this);
        no->setWindowTitle("alert");
        no->setText("Bitte mindestens eine speise hinzufügen");
        no->exec();
        return;
    }
    new_trainingseinheit_->datum_ = this->findChild<QDateEdit*>("date_edit")->date().toString(Qt::DateFormat::ISODate);
    new_trainingseinheit_->trainingsplan_id_ = tp_id;
    new_trainingseinheit_->user_id_ = Login::GetInstance()->GetLoggedInUser()->GetId();
    new_trainingseinheit_->durchgefuehrt = 0;
    new_trainingseinheit_->start_zeit_ = "00:00:00";
    new_trainingseinheit_->end_zeit_ = "00:00:00";

    new_trainingseinheit_->id_ = Trainingseinheit::AddTrainingseinheit(*new_trainingseinheit_);
    qDebug() << new_trainingseinheit_->uebungspositionen.size();
    this->findChild<QWidget*>("add_new_trainingseinheit")->show();
    this->findChild<QWidget*>("trainingseinheit_eingeben_widget")->hide();

    //delete speisepos widgets
    QList<QWidget*> uebungspos_to_delete = this->findChildren<QWidget*>("uebungspos_widget");
    for (QWidget* pos : uebungspos_to_delete) {
        delete pos;
    }
}

void TrainingseinheitEingeben::FillComboBox(QComboBox* box) {
    QStringList filler;
    QString prep = "SELECT Titel from Uebung ORDER BY Titel ASC;";
    QSqlQuery query;
    query.prepare(prep);
    query.exec();
    while (query.next()) {
        filler.push_back(query.value("Titel").toString());
    }
    box->addItems(filler);
}
