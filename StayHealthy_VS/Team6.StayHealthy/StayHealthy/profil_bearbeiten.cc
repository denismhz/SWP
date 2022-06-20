#include "profil_bearbeiten.h"
#include "registrierung.h"
#include "login.h"
#include <QMessageBox>
#include <QInputDialog>



ProfilBearbeiten::ProfilBearbeiten(QDialog*parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    User* tmpUser = Login::GetInstance()->GetLoggedInUser();
    currentUserProfil = Profil::GetProfil(*tmpUser);
    if (!currentUserProfil) {
        currentUserProfil = new Profil();
        currentUserProfil->geb_datum_ = "2000-10-10";
        currentUserProfil->name_ = "";
        currentUserProfil->vorname_ = "";
        currentUserProfil->geschlecht_ = 1;
        currentUserProfil->height_ = 100;
        currentUserProfil->weight_ = 70;
        currentUserProfil->kalorienaufnahme_ = 20000;
        currentUserProfil->prefaerenz_ = "";
        Profil::AddProfil(*currentUserProfil, Login::GetInstance()->GetLoggedInUser()->GetId());
    };

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

        if (!Profil::UpdateProfil(*currentUserProfil, Login::GetInstance()->GetLoggedInUser()->GetId())) {
        }
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

        if (password == Login::GetInstance()->GetLoggedInUser()->password_)
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
    ui.label_cal->hide();
    ui.label_pref->hide();
}

void ProfilBearbeiten::showLabels()
{
    ui.label_firstname->show();
    ui.label_lastname->show();
    ui.label_birthday->show();
    ui.label_gender->show();
    ui.label_height->show();
    ui.label_weight->show();
    ui.label_cal->show();
    ui.label_pref->show();
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
    ui.lineEdit_cal->hide();
    ui.lineEdit_pref->hide();
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
    ui.lineEdit_cal->show();
    ui.lineEdit_pref->show();
}

void ProfilBearbeiten::resetInputFields()
{
    ui.lineEdit_firstname->clear();
    ui.lineEdit_lastname->clear();
    ui.lineEdit_cal->clear();
    ui.lineEdit_pref->clear();
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
    ui.label_firstname->setText(currentUserProfil->vorname_);
    ui.label_lastname->setText(currentUserProfil->name_);


    ui.label_birthday->setText(currentUserProfil->geb_datum_ + " (" + QString::number(currentUserProfil->GetAlter()) + ")");

    if (currentUserProfil->geschlecht_ != 0)
        ui.label_gender->setText("maennlich");
    else
        ui.label_gender->setText("weiblich");

    ui.label_height->setText(QString::number(currentUserProfil->height_));
    ui.label_weight->setText(QString::number(currentUserProfil->weight_));
    ui.label_cal->setText(QString::number(currentUserProfil->kalorienaufnahme_));
    ui.label_pref->setText(currentUserProfil->prefaerenz_);
}

void ProfilBearbeiten::setUpdatedUserData()
{
    currentUserProfil->vorname_ = ui.lineEdit_firstname->text();
    currentUserProfil->name_ = ui.lineEdit_lastname->text();
    currentUserProfil->geb_datum_ = ui.dateEdit_birthday->date().toString("yyyy-MM-dd");
    currentUserProfil->kalorienaufnahme_ = ui.lineEdit_cal->text().toInt();
    currentUserProfil->prefaerenz_ = ui.lineEdit_pref->text();

    if (ui.radioButton_m->isChecked())
        currentUserProfil->geschlecht_ = 1;
    else
        currentUserProfil->geschlecht_ = 0;

    currentUserProfil->height_ = ui.spinBox_height->value();
    currentUserProfil->weight_ = ui.spinBox_weight->value();
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

    if (ui.lineEdit_cal->text().isEmpty())
    {
        msg.append("Bitte Kalorienaufnahme eingeben!\n");
        retVal = false;
    }

    if (ui.lineEdit_pref->text().isEmpty())
    {
        msg.append("Bitte Praeferenz eingeben!\n");
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
    ui.lineEdit_firstname->setText(currentUserProfil->vorname_);
    ui.lineEdit_lastname->setText(currentUserProfil->name_);
    ui.lineEdit_cal->setText(QString::number(currentUserProfil->kalorienaufnahme_));
    ui.lineEdit_pref->setText(currentUserProfil->prefaerenz_);
    ui.dateEdit_birthday->setDate(QDate::fromString(currentUserProfil->geb_datum_, "yyyy-MM-dd"));

    if (currentUserProfil->geschlecht_ == 0)
        ui.radioButton_w->setChecked(true);
    else
        ui.radioButton_m->setChecked(true);

    ui.spinBox_height->setValue(currentUserProfil->height_);
    ui.spinBox_weight->setValue(currentUserProfil->weight_);
}
