#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_stayhealthy.h"
#include "user.h"
#include "qstackedwidget.h"
#include "main_menu.h"

class StayHealthy : public QMainWindow
{
    Q_OBJECT

public:
    StayHealthy(QWidget *parent = Q_NULLPTR);
 QStackedWidget *widgets;
    QWidget *main;
 MainMenu *menu;
    

public slots:
 //User* login();
 void Register();

private:
    Ui::StayHealthyClass ui;
};
