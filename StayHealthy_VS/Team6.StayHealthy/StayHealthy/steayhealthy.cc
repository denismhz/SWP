#include "stayhealthy.h"
#include "db_handler.h"
#include "qmessagebox.h"
#include "registrierung.h"
#include "main_menu.h"
#include "login.h"

StayHealthy::StayHealthy(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    ui.setupUi(this);
    ui.stackedWidget->setCurrentIndex(0);
    
    //setup main menu
    menu = new MainMenu(this);
    ui.stackedWidget->addWidget(menu);

    //setup message boxes
    confirm_ = new QMessageBox(parent);
    confirm_->setWindowTitle("Confirm");
    alert_ = new QMessageBox(parent);
    alert_->setWindowTitle("Alert");


    //Connect Buttons
    connect(ui.toRegisterPage, SIGNAL(clicked()), this, SLOT(WelcomeRegisterPressed()));
    connect(ui.toLoginPage, SIGNAL(clicked()), this, SLOT(WelcomeLoginPressed()));
    connect(ui.registerButton, SIGNAL(clicked()), this, SLOT(RegisterPressed()));
    connect(ui.loginButton, SIGNAL(clicked()), this, SLOT(LoginPressed()));
    connect(ui.homeButton, SIGNAL(clicked()), this, SLOT(HomePressed()));
    connect(ui.homeButtonL, SIGNAL(clicked()), this, SLOT(HomePressed()));
}

void StayHealthy::WelcomeRegisterPressed()
{
    ui.stackedWidget->setCurrentIndex(2);
}

void StayHealthy::WelcomeLoginPressed()
{
    ui.stackedWidget->setCurrentIndex(1);
}

void StayHealthy::RegisterPressed() {
    QString email = ui.emailLineEdit_2->text();
    QString password = ui.passwordLineEdit_2->text();
    QString passwordwdh = ui.passwordwdh_lineedit->text();
    User* new_user = new User(email, password);
    if (new_user->email_.isEmpty() || new_user->password_.isEmpty()) {
        alert_->setText("Bitte alle Daten eingeben");
        alert_->exec();
    }
    else if (QString::compare(password, passwordwdh, Qt::CaseSensitive))
    {
        alert_->setText("Passw�rter stimmen nicht �berein!");
        alert_->exec();
    }
    else if(Registrierung::RegistriereBenutzer(*new_user)) {
        confirm_->setText("Sie haben sich erfolgreich registriert");
        confirm_->exec();
        ui.stackedWidget->setCurrentIndex(0);
    }
    else {
        alert_->setText("Registrierung fehlgeschlagen. Email adresse registriert oder falsch");
        alert_->exec();
    }
    //clear line edits
    QList<QLineEdit*> line_edits = ui.stackedWidget->currentWidget()->findChildren<QLineEdit*>();
    for (QLineEdit* le : line_edits) {
        le->clear();
    }
}

void StayHealthy::LoginPressed()
{
    QString email = ui.emailLineEdit->text();
    QString password = ui.passwordLineEdit->text();
    User* user = new User(email, password);
    if (user->email_.isEmpty() || user->password_.isEmpty()) {
        alert_->setText("Bitte alle Daten eingeben");
        alert_->exec();
    }
    
    else if (!Login::GetInstance()->LoginUser(*user)) {
        alert_->setText("Login fehlgeschlagen. Email adresse oder passwort falsch");
        alert_->exec();
        
    }
    else {
        confirm_->setText("Sie haben sich erfolgreich eingeloggt");
        confirm_->exec();
        ui.stackedWidget->setCurrentWidget(menu);
    }
    QList<QLineEdit*> line_edits = ui.stackedWidget->currentWidget()->findChildren<QLineEdit*>();
    for (QLineEdit* le : line_edits) {
        le->clear();
    }

    //hier weiterleiten zum profil ausf�llen dann sportler updaten
    //Note: email lookup is case insensitive
}

void StayHealthy::HomePressed()
{
    ui.stackedWidget->setCurrentIndex(0);
}





