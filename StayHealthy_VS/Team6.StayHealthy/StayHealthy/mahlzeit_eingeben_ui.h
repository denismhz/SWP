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
	MahlzeitEingebenUI(QWidget *parent, int e_plan_id);
	~MahlzeitEingebenUI();
	int e_plan_id_;
	QWidget* scroll_widget_;
	Mahlzeit* new_mahlzeit;

private slots:
	void ShowEingabe();
	void AddSpeisePos();
	void Finish();

private:
	QScrollArea* speisepos_area;
	void FillComboBox(QComboBox* box);
};
