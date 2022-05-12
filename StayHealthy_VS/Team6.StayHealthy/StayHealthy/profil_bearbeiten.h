#pragma once

#include <QDialog>
#include "ui_profil_bearbeiten.h"

class ProfilBearbeiten : public QDialog
{
	Q_OBJECT

public:
	ProfilBearbeiten(QDialog *parent = Q_NULLPTR);
	~ProfilBearbeiten();

private slots:
	void on_pushButtonClose_clicked();

signals:
	void profileWindow_Closing();

private:
	Ui::ProfilBearbeiten ui;
};
