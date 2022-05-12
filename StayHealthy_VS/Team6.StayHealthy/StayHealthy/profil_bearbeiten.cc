#include "profil_bearbeiten.h"

ProfilBearbeiten::ProfilBearbeiten(QDialog*parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButtonClose, SIGNAL(clicked()), this, SLOT(on_pushButtonClose_clicked()));


    ui.label_firstname->setText("vorname");
    ui.lineEdit_lastname->setText("nachname");
    ui.label_birthday->setText("geb");
    ui.label_gender->setText("geschl");
    ui.label_height->setText("gr");
    ui.label_weight->setText("gew");

    ui.lineEdit_firstname->hide();
    ui.lineEdit_lastname->hide();
    ui.dateEdit_birthday->hide();
    ui.radioButton_m->hide();
    ui.radioButton_w->hide();
    ui.spinBox_height->hide();
    ui.doubleSpinBox_weight->hide();

}

ProfilBearbeiten::~ProfilBearbeiten()
{

}

void ProfilBearbeiten::on_pushButtonClose_clicked()
{
    emit profileWindow_Closing();
}
