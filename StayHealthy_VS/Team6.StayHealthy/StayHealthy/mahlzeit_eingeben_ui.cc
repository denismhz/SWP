#include "mahlzeit_eingeben_ui.h"
#include "qpushbutton.h"
#include "qstringlist.h"
#include "qmessagebox.h"
#include "db_handler.h"
#include "qlineedit.h"
#include "mahlzeit.h"
#include "qlayout.h"
#include "qlabel.h"
#include "qdatetime.h"
#include "registrierung.h"
#include "login.h"

MahlzeitEingebenUI::MahlzeitEingebenUI(QWidget *parent)
    : QWidget(parent)
{
    speisepos_area = new QScrollArea(this);
    new_mahlzeit = nullptr;

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignTop);

    QPushButton* add_new_mahlzeit = new QPushButton("Neue Mahlzeit erstellen", this);
    add_new_mahlzeit->setObjectName("add_new_mahlzeit");
    connect(add_new_mahlzeit, SIGNAL(clicked()), this, SLOT(ShowEingabe()));
    main_layout->addWidget(add_new_mahlzeit);

    QWidget* mahlzeit_eingeben_widget = new QWidget(this);
    mahlzeit_eingeben_widget->setObjectName("mahlzeit_eingeben_widget");
    main_layout->addWidget(mahlzeit_eingeben_widget);
    mahlzeit_eingeben_widget->hide();

    main_layout->addWidget(speisepos_area);
    speisepos_area->setWidget(this->findChild<QWidget*>("speisepos_widget"));

    QHBoxLayout* eingabe_layout = new QHBoxLayout(mahlzeit_eingeben_widget);
       
    QComboBox* speisen = new QComboBox(mahlzeit_eingeben_widget);
    speisen->setObjectName("speisen");
    eingabe_layout->addWidget(speisen);
    FillComboBox(speisen);

    QLineEdit* menge = new QLineEdit("100", mahlzeit_eingeben_widget);
    QLabel* gramm = new QLabel("g", mahlzeit_eingeben_widget);
    menge->setObjectName("menge");
    menge->setAlignment(Qt::AlignRight);
    menge->setFixedWidth(160);
    eingabe_layout->addWidget(menge);
    eingabe_layout->addWidget(gramm);

    QPushButton* add_speise_button = new QPushButton("Hinzufuegen", mahlzeit_eingeben_widget);
    connect(add_speise_button, SIGNAL(clicked()), this, SLOT(AddSpeisePos()));
    add_speise_button->setFixedWidth(100);
    eingabe_layout->addWidget(add_speise_button);

    QPushButton* done = new QPushButton("Fertig", this);
    connect(done, SIGNAL(clicked()), this, SLOT(Finish()));
    main_layout->addWidget(done);
}

MahlzeitEingebenUI::~MahlzeitEingebenUI()
{
}

void MahlzeitEingebenUI::ShowEingabe() {
    this->findChild<QWidget*>("add_new_mahlzeit")->hide();
    this->findChild<QWidget*>("mahlzeit_eingeben_widget")->show();
    new_mahlzeit = new Mahlzeit();
}

void MahlzeitEingebenUI::AddSpeisePos()
{
    Speiseposition* speisepos = new Speiseposition();
    speisepos->menge_ = this->findChild<QLineEdit*>("menge")->text().toInt();
    speisepos->beschreibung_ = this->findChild<QComboBox*>("speisen")->currentText();

    //get speiseid
    QString prep = "SELECT SpeiseID from Speise WHERE Titel = :titel;";
    QSqlQuery query;
    query.prepare(prep);
    query.bindValue(":titel", speisepos->beschreibung_);
    query.exec();
    query.first();
    speisepos->speise_id_ = query.value(0).toInt();

    QWidget* speisepos_widget = new QWidget(this);
    speisepos_widget->setObjectName("speisepos_widget");
    this->layout()->addWidget(speisepos_widget);

    QHBoxLayout* speisepos_layout = new QHBoxLayout(speisepos_widget);
    new_mahlzeit->speisepositionen.push_back(speisepos);
    qDebug() << new_mahlzeit->speisepositionen.size();

    QLabel* speisepos_beschreibung = new QLabel(speisepos->beschreibung_, speisepos_widget);
    QLabel* speisepos_menge = new QLabel(QString::number(speisepos->menge_), speisepos_widget);
    speisepos_layout->addWidget(speisepos_beschreibung);
    speisepos_layout->addWidget(speisepos_menge);
}

void MahlzeitEingebenUI::Finish()
{
    if (!new_mahlzeit) {
        QMessageBox* no = new QMessageBox(this);
        no->setWindowTitle("alert");
        no->setText("Bitte Mahlzeit erstellen");
        no->exec();
        return;
    }
    else if (new_mahlzeit->speisepositionen.size() <= 0) {
        QMessageBox* no = new QMessageBox(this);
        no->setWindowTitle("alert");
        no->setText("Bitte mindestens eine speise hinzufügen");
        no->exec();
        return;
    }
    new_mahlzeit->datum_ = QDate::currentDate().toString(Qt::DateFormat::ISODate);
    new_mahlzeit->uhrzeit_ = QTime::currentTime().toString();
    new_mahlzeit->ernaehrungsplan_id_ = 0;
    new_mahlzeit->user_id_ = Login::GetInstance()->GetLoggedInUser()->GetId();
    new_mahlzeit->id_ = DBHandler::GetInstance()->AddMahlzeit(*new_mahlzeit);
    qDebug() << new_mahlzeit->speisepositionen.size();
    this->findChild<QWidget*>("add_new_mahlzeit")->show();
    this->findChild<QWidget*>("mahlzeit_eingeben_widget")->hide();

    //delete speisepos widgets
    QList<QWidget*> speisepos_to_delete = this->findChildren<QWidget*>("speisepos_widget");
    for (QWidget* spos : speisepos_to_delete) {
        delete spos;
    }
}

void MahlzeitEingebenUI::FillComboBox(QComboBox* box) {
    QStringList filler;
    QString prep = "SELECT Titel from Speise ORDER BY Titel ASC;";
    QSqlQuery query;
    query.prepare(prep);
    query.exec();
    while (query.next()) {
        filler.push_back(query.value("Titel").toString());
    }
    box->addItems(filler);
}
