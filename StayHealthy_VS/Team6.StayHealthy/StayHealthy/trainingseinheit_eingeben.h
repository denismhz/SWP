#pragma once

#include <QWidget>
#include "trainingseinheit.h"
#include "qscrollarea.h"
#include "qcombobox.h"

class TrainingseinheitEingeben : public QWidget
{
	Q_OBJECT

private:
	Trainingseinheit* new_trainingseinheit_;
	QWidget* scroll_widget_;
	int tp_id;

public:
	TrainingseinheitEingeben(QWidget *parent, int tp_id);
	~TrainingseinheitEingeben();

private slots:
	void ShowEingabe();
	void AddUebungsPos();
	void Finish();

private:
	QScrollArea* uebungspos_area;
	void FillComboBox(QComboBox* box);
};
