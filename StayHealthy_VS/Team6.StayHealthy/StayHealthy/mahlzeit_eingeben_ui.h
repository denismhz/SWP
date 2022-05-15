#pragma once

#include <QWidget>
#include <QSql>
#include "qcombobox.h"
#include "mahlzeit.h"
#include "qscrollarea.h"

class MahlzeitEingebenUI : public QWidget
{
	Q_OBJECT

public:
	MahlzeitEingebenUI(QWidget *parent);
	~MahlzeitEingebenUI();

private slots:
	void ShowEingabe();
	void AddSpeisePos();
	void Finish();

private:
	Mahlzeit* new_mahlzeit;
	QScrollArea* speisepos_area;
	void FillComboBox(QComboBox* box);
};
