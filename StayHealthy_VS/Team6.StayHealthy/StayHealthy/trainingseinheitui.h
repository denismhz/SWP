#pragma once

#include <QWidget>
#include "ui_trainingseinheitui.h"
#include <vector>
#include "trainingseinheit.h"
#include "db_handler.h"
#include "qstackedwidget.h"
#include "qlineedit.h"
#include <QtSql>
#include <qtimer.h>
#include "qlabel.h"

class TrainingseinheitUI : public QWidget
{
	Q_OBJECT

public:
	TrainingseinheitUI(QWidget *parent = Q_NULLPTR);
	~TrainingseinheitUI();
	void SetUp();

public slots:
	void NextTe();
	void PrevTe();
	void Start();
	void Cancel();
	void Stop();
	void Finish();
	
private slots:
	void UpdateLabel();

private:
	Ui::TrainingseinheitUI ui;
	QStackedWidget* stack;
	QSqlQuery* query;
	Trainingseinheit* curr_te;
	QTimer* timerr;
	int sec;

	void SetUpTeUI(Trainingseinheit&);
};