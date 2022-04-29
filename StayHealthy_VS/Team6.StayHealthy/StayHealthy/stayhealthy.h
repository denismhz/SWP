#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_stayhealthy.h"

class StayHealthy : public QMainWindow
{
    Q_OBJECT

public:
    StayHealthy(QWidget *parent = Q_NULLPTR);
private:
    Ui::StayHealthyClass ui;
};
