#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_stayhealthy.h"
#include "qmessagebox.h"

class StayHealthy : public QMainWindow
{
    Q_OBJECT

public:
    StayHealthy(QWidget *parent = Q_NULLPTR);
    QMessageBox* alert_;
    QMessageBox* confirm_;
    QWidget* menu;

public slots:
   
    void WelcomeRegisterPressed();
    void WelcomeLoginPressed();
    void RegisterPressed();
    void LoginPressed();
    void HomePressed();

private:
    Ui::StayHealthyClass ui;
};
