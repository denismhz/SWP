#include "stayhealthy.h"
#include "db_handler.h"
#include "qmessagebox.h"
#include "registrierung.h"
#include "main_menu.h"
#include "login.h"

StayHealthy::StayHealthy(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignCenter);

    stack_ = new QStackedWidget(this);
    QVBoxLayout* stack_layout = new QVBoxLayout(stack_);
    main_layout->addWidget(stack_);
    
    stack_->addWidget(SetupWelcomeUI());
    stack_->addWidget(SetupRegisterUI());
    stack_->addWidget(SetupLoginUI());




    //setup main menu
    menu = new MainMenu(stack_);
    stack_->addWidget(menu);

    //setup message boxes
    confirm_ = new QMessageBox(parent);
    confirm_->setWindowTitle("Confirm");
    alert_ = new QMessageBox(parent);
    alert_->setWindowTitle("Alert");

    QPushButton* back_button = new QPushButton("Zuruck", this);
    back_button->setObjectName("back");
    
    main_layout->addWidget(back_button);
    
   
    this->setStyleSheet("QPushButton {display: inline-block; outline : 0;"
                        "border: none; cursor: pointer; font-weight: 600;"
                        "border-radius: 4px; font-size: 13px; height: 30px;"
                        "border-width: 1px;"
                        "background-color: lightgrey; color: black; padding: 0 20px;}"
                        "QPushButton:hover{background-color: darkgrey;}"
                        "QLabel {font-size: 13px; font-weight: 600;}"
                        "QLineEdit { border: none; }");
   

    stack_->setCurrentIndex(0);

    //Connect Buttons


    connect(back_button, SIGNAL(clicked()), this, SLOT(HomePressed()));
    //connect(ui.homeButtonL, SIGNAL(clicked()), this, SLOT(HomePressed()));
}

void StayHealthy::WelcomeRegisterPressed()
{
    stack_->setCurrentIndex(1);
}

void StayHealthy::WelcomeLoginPressed()
{
    stack_->setCurrentIndex(2);
}

void StayHealthy::RegisterPressed() {
   
    QString password = stack_->currentWidget()->findChild<QLineEdit*>("reg_pass")->text();
    QString passwordwdh = stack_->currentWidget()->findChild<QLineEdit*>("reg_pass_wdh")->text();
    QString email = stack_->currentWidget()->findChild<QLineEdit*>("reg_email")->text();

    //clear line edits
    QList<QLineEdit*> line_edits = stack_->currentWidget()->findChildren<QLineEdit*>();
    for (QLineEdit* le : line_edits) {
        le->clear();
    }

    User* new_user = new User(email, password);
    if (new_user->email_.isEmpty() || new_user->password_.isEmpty()) {
        alert_->setText("Bitte alle Daten eingeben");
        alert_->exec();
    }
    else if (QString::compare(password, passwordwdh, Qt::CaseSensitive))
    {
        alert_->setText("Passwörter stimmen nicht überein!");
        alert_->exec();
    }
    else if(Registrierung::RegistriereBenutzer(*new_user)) {
        confirm_->setText("Sie haben sich erfolgreich registriert");
        confirm_->exec();
        stack_->setCurrentIndex(0);
    }
    else {
        alert_->setText("Registrierung fehlgeschlagen. Email adresse registriert oder falsch");
        alert_->exec();
    }
    
    delete new_user;
}

void StayHealthy::LoginPressed()
{
    QString email = stack_->currentWidget()->findChild<QLineEdit*>("login_email")->text();
    QString password = stack_->currentWidget()->findChild<QLineEdit*>("login_pass")->text();
    qDebug() << email << password << ":::asd";

    QList<QLineEdit*> line_edits = stack_->currentWidget()->findChildren<QLineEdit*>();
    for (QLineEdit* le : line_edits) {
        le->clear();
    }
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
        stack_->setCurrentWidget(menu);
        this->findChild<QPushButton*>("back")->hide();
        confirm_->exec();
    }
    delete user;
    //hier weiterleiten zum profil ausfüllen dann sportler updaten
    //Note: email lookup is case insensitive
}

void StayHealthy::HomePressed()
{
    stack_->setCurrentIndex(0);
}

QWidget* StayHealthy::SetupLoginUI() {
    
    QWidget* login_page = new QWidget(stack_);
    QFormLayout* form = new QFormLayout(login_page);

    QVBoxLayout* login_page_layout = new QVBoxLayout(login_page);
    login_page_layout->setAlignment(Qt::AlignCenter);
   
    QLabel* email_label = new QLabel("Email: ");
    login_page_layout->addWidget(email_label);
    QLineEdit* login_email = new QLineEdit(login_page);
    login_email->setObjectName("login_email");
    login_page_layout->addWidget(login_email);
    form->addRow(email_label, login_email);

    QLabel* password_label = new QLabel("Passwort: ");
    login_page_layout->addWidget(password_label);
    QLineEdit* login_pass = new QLineEdit(login_page);
    login_pass->setEchoMode(QLineEdit::Password);
    login_pass->setObjectName("login_pass");
    login_page_layout->addWidget(login_pass);
    form->addRow(password_label, login_pass);

    QPushButton* login_button = new QPushButton("Login", login_page);
    login_page_layout->addWidget(login_button);
    form->addRow(login_button);
   
    connect(login_button, SIGNAL(clicked()), this, SLOT(LoginPressed()));

    return login_page;
}

QWidget* StayHealthy::SetupWelcomeUI()
{
    QWidget* welcome_page = new QWidget(stack_);
    QHBoxLayout* welcome_page_layout = new QHBoxLayout(welcome_page);
    QPushButton* toRegisterPage = new QPushButton("Registrieren", welcome_page);
    QPushButton* toLoginPage = new QPushButton("Login", welcome_page);
    welcome_page_layout->addWidget(toRegisterPage);
    welcome_page_layout->addWidget(toLoginPage);

    connect(toRegisterPage, SIGNAL(clicked()), this, SLOT(WelcomeRegisterPressed()));
    connect(toLoginPage, SIGNAL(clicked()), this, SLOT(WelcomeLoginPressed()));

    return welcome_page;
}

QWidget* StayHealthy::SetupRegisterUI()
{
    
    QWidget* register_page = new QWidget(stack_);
    QFormLayout* form = new QFormLayout(register_page);

    QVBoxLayout* register_page_layout = new QVBoxLayout(register_page);

    QLabel* email_label = new QLabel("Email: ");
    QLineEdit* reg_email = new QLineEdit(register_page);
    register_page_layout->addWidget(reg_email);
    reg_email->setObjectName("reg_email");
    
    form->addRow(email_label, reg_email);

    QLabel* password_label = new QLabel("Passwort: ");
    QLineEdit* reg_pass = new QLineEdit(register_page);
    reg_pass->setEchoMode(QLineEdit::Password);
    reg_pass->setObjectName("reg_pass");
    register_page_layout->addWidget(reg_pass);
    form->addRow(password_label, reg_pass);

    QLabel* password_wdh_label = new QLabel("Passwort Wiederholen: ");
    QLineEdit* reg_pass_wdh = new QLineEdit(register_page);
    reg_pass_wdh->setEchoMode(QLineEdit::Password);
    reg_pass_wdh->setObjectName("reg_pass_wdh");
    register_page_layout->addWidget(reg_pass_wdh);
    form->addRow(password_wdh_label, reg_pass_wdh);

    QPushButton* registerButton = new QPushButton("Registrieren");
    register_page_layout->addWidget(registerButton);
    form->addRow(registerButton);

    connect(registerButton, SIGNAL(clicked()), this, SLOT(RegisterPressed()));

    return register_page;
}





