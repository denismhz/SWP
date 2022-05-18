#pragma once

#include <QDialog>
#include <QButtonGroup>
#include "ui_profil_bearbeiten.h"
#include "profil.h"

class ProfilBearbeiten : public QDialog
{
	Q_OBJECT

public:
	ProfilBearbeiten(QDialog *parent = Q_NULLPTR);
	~ProfilBearbeiten();

private slots:
	void on_pushButtonClose_clicked();
	void on_pushButtonEditProfile_clicked();
	void on_pushButtonSaveChanges_clicked();
	void on_pushButtonDiscardChanges_clicked();
	void on_pushButtonDeleteProfile_clicked();

signals:
	void profileWindow_Closing();
	void profileDeleted();

private:
	Ui::ProfilBearbeiten ui;
	QButtonGroup genderGroup;

	void hideLabels();
	void showLabels();
	void hideInputFields();
	void showInputFields();
	void resetInputFields();
	void refreshLabels();
	void setUpdatedUserData();
	bool inputsAreValid();
	void fillInputFields();

	int userID;
	Profil* currentUserProfil;
};
