#include "trainingseinheitui.h"
#include "db_handler.h"
#include "user.h"
#include "registrierung.h"
#include "qlayout.h"
#include "qpushbutton.h"
#include "qstackedwidget.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qstring.h"
#include "login.h"

TrainingseinheitUI::TrainingseinheitUI(QWidget *parent)
    : QWidget(parent)
{  
    timerr = new QTimer(this);
    query = nullptr;
    sec = 0;

    curr_te = nullptr;
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    QHBoxLayout* button_layout = new QHBoxLayout(this);
    main_layout->addLayout(button_layout);
    
    //stacked widget für einheiten
    stack = new QStackedWidget(this);
    main_layout->addWidget(stack);
   
    QPushButton* nextTe = new QPushButton("next", this);
    QPushButton* prevTe = new QPushButton("previous", this);
    QPushButton* start = new QPushButton("Start", this);
    QPushButton* stop = new QPushButton("Stop", this);
    QPushButton* fertig = new QPushButton("Fertig", this);

    button_layout->addWidget(start);
    button_layout->addWidget(stop);
    button_layout->addWidget(fertig);
    button_layout->addWidget(prevTe);
    button_layout->addWidget(nextTe);

    connect(nextTe, SIGNAL(clicked()), this, SLOT(NextTe()));
    connect(prevTe, SIGNAL(clicked()), this, SLOT(PrevTe()));
    connect(fertig, SIGNAL(clicked()), this, SLOT(Finish()));
    connect(start, SIGNAL(clicked()), this, SLOT(Start()));
    connect(stop, SIGNAL(clicked()), this, SLOT(Stop()));
    connect(timerr, SIGNAL(timeout()), this, SLOT(UpdateLabel()));
}

TrainingseinheitUI::~TrainingseinheitUI()
{
    delete curr_te;
    timerr->deleteLater();
    stack->deleteLater();
    delete query;
}

void TrainingseinheitUI::PrevTe()
{
    if (!query->previous()) {
        return;
    };
    Trainingseinheit* te = new Trainingseinheit();
    te->datum_ = query->value("Datum").toString();
    te->start_zeit_ = query->value("Startzeit").toString();
    te->end_zeit_ = query->value("Endzeit").toString();
    te->durchgefuehrt = query->value("Durchgefuehrt").toBool();
    te->kalorienverbrauch = query->value("Kalorienverbrauch").toInt();
    te->id_ = query->value("TrainingseinheitID").toInt();
    te->uebungspositionen = Uebungsposition::GetUebungsposition(te->id_);
    curr_te = te;
    qDebug() << curr_te->id_;
    //qDebug() << query->value("TrainingseinheitID");
    Stop();
    sec = 0;
    stack->setCurrentIndex(stack->currentIndex() - 1);
}

void TrainingseinheitUI::SetUp() {
    User* current_user = Login::GetInstance()->GetLoggedInUser();
    //pls sort einheiten by datum
    QString prep = "select * from Trainingseinheit "
        "WHERE BenutzerID = :user_id AND Durchgefuehrt = 0 ORDER BY Datum";

    for (int i = stack->count(); i >= 0; i--) {
        QWidget* widget = stack->widget(i);
        if(widget) stack->removeWidget(widget);
        widget->deleteLater();
    }
    query = new QSqlQuery();
    query->prepare(prep);
    query->bindValue(":user_id", Login::GetInstance()->GetLoggedInUser()->GetId());
    query->exec();
    //stack->setCurrentIndex(0);
    NextTe();
}

void TrainingseinheitUI::UpdateLabel()
{

    //neue zeit zu alter zeit im label hinzufügen fehlt noch
    qDebug() << sec;
    sec++;
    int min = (sec / 60) % 60;
    int h = (sec / 3600);
    //add zero before if < 10
    QString time = QDateTime::fromSecsSinceEpoch(sec).toUTC().toString("hh:mm:ss");
    time = "Stoppuhr " + time;
    stack->currentWidget()->findChild<QLabel*>("timer_label")->setText(time);
}

void TrainingseinheitUI::Start()
{
    if (!curr_te) return;
    timerr->start(1000);
}

void TrainingseinheitUI::Cancel()
{
   
}

void TrainingseinheitUI::Stop()
{
    if (!curr_te) return;
    timerr->stop();
}

void TrainingseinheitUI::Finish()
{
    if (!curr_te) return;
    Stop();
    QList<QLineEdit*> line_edits = stack->currentWidget()->findChildren<QLineEdit*>();

    for (int i = 0; i < line_edits.size(); i++) {
        int menge = line_edits[i]->text().toInt();
        curr_te->uebungspositionen[i]->menge_ = menge;
        Uebungsposition::UpdateUebungsposition(*curr_te->uebungspositionen[i]);
    }
    //update trainingseinheit druchgefuehrt
    QString prep1 =
        QString("update Trainingseinheit set Startzeit = DATEADD(second, %0, Startzeit)"
            "where TrainingseinheitID = %1").arg(sec).arg(curr_te->id_);
    qDebug() << "curr id" << curr_te->id_;
    DBHandler::GetInstance()->CustomQuery(prep1);
    QString prep = QString("Update dbo.Trainingseinheit Set Durchgefuehrt = 1 WHERE TrainingseinheitID = %1").arg(curr_te->id_);
    DBHandler::GetInstance()->CustomQuery(prep);
    curr_te = nullptr;
    SetUp();
}

void TrainingseinheitUI::SetUpTeUI(Trainingseinheit& te)
{
    
    QWidget* te_widget = new QWidget(this);
    QVBoxLayout* te_main_layout = new QVBoxLayout(te_widget);

    QString te_header_txt = QString("<h3>Trainingseinheit %0 am %1</h3>").arg(te.id_).arg(te.datum_);

    QLabel* te_header_label = new QLabel(te_header_txt, te_widget);
    te_main_layout->addWidget(te_header_label);
    QLabel* timer_label = new QLabel(te_widget);
    timer_label->setText("Stoppuhr 00:00:00");
    timer_label->setObjectName("timer_label");
    te_main_layout->addWidget(timer_label);

    //übungspositionen durchgehen und widgets dafür erstellen

    //add label for startzeit endzeit und oder datum ändern

    for (auto ue : te.uebungspositionen) {
        QWidget* ue_widget = new QWidget(te_widget);
        QHBoxLayout* te_ue_layout = new QHBoxLayout(ue_widget);
        te_main_layout->addWidget(ue_widget);

        QString ue_header_txt = "" + ue->beschreibung_ + "";
        QLabel* ue_header = new QLabel(ue_header_txt, ue_widget);
        QLineEdit* ue_edit = new QLineEdit(QString::number(ue->menge_), ue_widget);
        ue_edit->setAlignment(Qt::AlignRight);
        ue_edit->setFixedSize(40, 20);
        QString ue_wdh_txt = " /" + QString::number(ue->menge_) + "";
        QLabel* ue_wdh = new QLabel(ue_wdh_txt, ue_widget);

        te_ue_layout->addWidget(ue_header);
        te_ue_layout->addWidget(ue_edit);
        te_ue_layout->addWidget(ue_wdh);
    }
    //only add widget once
    stack->addWidget(te_widget);
}

void TrainingseinheitUI::NextTe() {
    if(!query->next()) return;
    //qDebug() << query->value("TrainingseinheitID").toString();
    Trainingseinheit* te = new Trainingseinheit();
    te->datum_ = query->value("Datum").toString();
    te->start_zeit_ = query->value("Startzeit").toString();
    te->end_zeit_ = query->value("Endzeit").toString();
    te->durchgefuehrt = query->value("Durchgefuehrt").toBool();
    te->kalorienverbrauch = query->value("Kalorienverbrauch").toInt();
    te->id_ = query->value("TrainingseinheitID").toInt();
    te->uebungspositionen = Uebungsposition::GetUebungsposition(te->id_);
    curr_te = te;
    qDebug() << curr_te->id_;
    if (stack->currentIndex() + 1 >= stack->count()) SetUpTeUI(*te);
    Stop();
    sec = 0;
    stack->setCurrentIndex(stack->currentIndex() + 1);
}
