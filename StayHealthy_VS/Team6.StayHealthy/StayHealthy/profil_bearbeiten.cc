#include "profil_bearbeiten.h"
#include "registrierung.h"
#include "db_handler.h"
#include "login.h"
#include <QMessageBox>
#include <QInputDialog>



ProfilBearbeiten::ProfilBearbeiten(QDialog*parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    //connect(ui.pushButtonClose, SIGNAL(clicked()), this, SLOT(on_pushButtonClose_clicked()));
    //connect(ui.pushButtonEditProfile, SIGNAL(clicked()), this, SLOT(on_pushButtonEditProfile_clicked()));
    //connect(ui.pushButtonSaveChanges, SIGNAL(clicked()), this, SLOT(on_pushButtonSaveChanges_clicked()));
    //connect(ui.pushButtonDiscardChanges, SIGNAL(clicked()), this, SLOT(on_pushButtonDiscardChanges_clicked()));


    User* tmpUser = Login::GetInstance()->GetLoggedInUser();

    currentUser = DBHandler::GetInstance()->GetSportlerByUserId(tmpUser->GetId());
    //currentUser->name_ = tmpUser->name_;
    //currentUser->vorname_ = tmpUser->vorname_;
    currentUser->SetId(tmpUser->GetId());
    currentUser->email_ = tmpUser->email_;
    currentUser->password_ = tmpUser->password_;


    genderGroup.addButton(ui.radioButton_m);
    genderGroup.addButton(ui.radioButton_w);

    refreshLabels();

    hideInputFields();
    
    ui.pushButtonDiscardChanges->hide();
    ui.pushButtonSaveChanges->hide();

}

ProfilBearbeiten::~ProfilBearbeiten()
{

}

void ProfilBearbeiten::on_pushButtonClose_clicked()
{
    emit profileWindow_Closing();
}

void ProfilBearbeiten::on_pushButtonEditProfile_clicked()
{
    showInputFields();
    hideLabels();
    fillInputFields();

    ui.pushButtonDiscardChanges->show();
    ui.pushButtonSaveChanges->show();

    ui.pushButtonEditProfile->hide();
    ui.pushButtonClose->hide();
    ui.pushButtonUpgradeProfile->hide();
}


void ProfilBearbeiten::on_pushButtonSaveChanges_clicked()
{
    if (inputsAreValid())
    {
        setUpdatedUserData();
        refreshLabels();
        hideInputFields();
        showLabels();

        ui.pushButtonSaveChanges->hide();
        ui.pushButtonDiscardChanges->hide();

        ui.pushButtonEditProfile->show();
        ui.pushButtonClose->show();
        ui.pushButtonUpgradeProfile->show();

        //TODO
        currentUser->kalorienaufnahme_ = 0;
        currentUser->prefaerenz_ = "";

        DBHandler::GetInstance()->UpdateSportlerByUserId(currentUser->GetId(), *currentUser);
    }    
}

void ProfilBearbeiten::on_pushButtonDiscardChanges_clicked()
{
    hideInputFields();
    showLabels();

    ui.pushButtonSaveChanges->hide();
    ui.pushButtonDiscardChanges->hide();

    ui.pushButtonEditProfile->show();
    ui.pushButtonClose->show();
    ui.pushButtonUpgradeProfile->show();
}

void ProfilBearbeiten::on_pushButtonDeleteProfile_clicked()
{
    bool isPasswordCorrect = false;
    bool isOk = false;
    QString password;

    while (isPasswordCorrect == false)
    {
        password = QInputDialog::getText(this, "Profil loeschen", "Zum Loeschen bitte Passwort bestaetigen:", QLineEdit::Password, "", &isOk);

        if (isOk == false)
            break;

        if (password == currentUser->password_)
        {
            isPasswordCorrect = true;
        }            
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Bitte richtiges Passwort eigeben!");
            msgBox.exec();
        }        
    }
}

void ProfilBearbeiten::hideLabels()
{
    ui.label_firstname->hide();
    ui.label_lastname->hide();
    ui.label_birthday->hide();
    ui.label_gender->hide();
    ui.label_height->hide();
    ui.label_weight->hide();
}

void ProfilBearbeiten::showLabels()
{
    ui.label_firstname->show();
    ui.label_lastname->show();
    ui.label_birthday->show();
    ui.label_gender->show();
    ui.label_height->show();
    ui.label_weight->show();
}

void ProfilBearbeiten::hideInputFields()
{
    ui.lineEdit_firstname->hide();
    ui.lineEdit_lastname->hide();
    ui.dateEdit_birthday->hide();
    ui.radioButton_m->hide();
    ui.radioButton_w->hide();
    ui.spinBox_height->hide();
    ui.spinBox_weight->hide();
}

void ProfilBearbeiten::showInputFields()
{
    //resetInputFields();

    ui.lineEdit_firstname->show();
    ui.lineEdit_lastname->show();
    ui.dateEdit_birthday->show();
    ui.radioButton_m->show();
    ui.radioButton_w->show();
    ui.spinBox_height->show();
    ui.spinBox_weight->show();
}

void ProfilBearbeiten::resetInputFields()
{
    ui.lineEdit_firstname->clear();
    ui.lineEdit_lastname->clear();
    ui.dateEdit_birthday->setDate(QDate(2000, 01, 01));

    genderGroup.setExclusive(false);
    ui.radioButton_m->setChecked(false);
    ui.radioButton_w->setChecked(false);
    genderGroup.setExclusive(true);

    ui.spinBox_height->setValue(ui.spinBox_height->minimum());
    ui.spinBox_weight->setValue(ui.spinBox_weight->minimum());
}

void ProfilBearbeiten::refreshLabels()
{
    ui.label_firstname->setText(currentUser->vorname_);
    ui.label_lastname->setText(currentUser->name_);


    ui.label_birthday->setText(currentUser->geb_datum_ + " (" + QString::number(currentUser->GetAlter()) + ")");

    if (currentUser->geschlecht_ != 0)
        ui.label_gender->setText("maennlich");
    else
        ui.label_gender->setText("weiblich");

    ui.label_height->setText(QString::number(currentUser->height_));
    ui.label_weight->setText(QString::number(currentUser->weight_));
}

void ProfilBearbeiten::setUpdatedUserData()
{
    currentUser->vorname_ = ui.lineEdit_firstname->text();
    currentUser->name_ = ui.lineEdit_lastname->text();
    currentUser->geb_datum_ = ui.dateEdit_birthday->date().toString("yyyy-MM-dd");

    if (ui.radioButton_m->isChecked())
        currentUser->geschlecht_ = 1;
    else
        currentUser->geschlecht_ = 0;

    currentUser->height_ = ui.spinBox_height->value();
    currentUser->weight_ = ui.spinBox_weight->value();
}

bool ProfilBearbeiten::inputsAreValid()
{
    bool retVal = true;
    QString msg = "";

    if (ui.lineEdit_firstname->text().isEmpty())
    {
        msg.append("Bitte Vorname eingeben!\n");
        retVal = false;
    }
        
    if (ui.lineEdit_lastname->text().isEmpty())
    {
        msg.append("Bitte Nachname eingeben!\n");
        retVal = false;
    }

    if (!ui.radioButton_m->isChecked() && !ui.radioButton_w->isChecked())
    {
        msg.append("Bitte Geschlecht auswaehlen!");
        retVal = false;
    }
       


    if (retVal == false)
    {
        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.setWindowTitle("Eingabefehler");
        msgBox.exec();
    }

    return retVal;
}

void ProfilBearbeiten::fillInputFields()
{
    ui.lineEdit_firstname->setText(currentUser->vorname_);
    ui.lineEdit_lastname->setText(currentUser->name_);
    ui.dateEdit_birthday->setDate(QDate::fromString(currentUser->geb_datum_, "yyyy-MM-dd"));

    if (currentUser->geschlecht_ == 0)
        ui.radioButton_w->setChecked(true);
    else
        ui.radioButton_m->setChecked(true);

    ui.spinBox_height->setValue(currentUser->height_);
    ui.spinBox_weight->setValue(currentUser->weight_);
}
