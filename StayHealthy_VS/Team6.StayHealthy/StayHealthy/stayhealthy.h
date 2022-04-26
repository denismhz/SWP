#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StayHealthy.h"
#include "user.h"

class StayHealthy : public QMainWindow
{
    Q_OBJECT

public:
    StayHealthy(QWidget *parent = Q_NULLPTR);

public slots:
 //User* login();
 void Register();

private:
    Ui::StayHealthyClass ui;
};
