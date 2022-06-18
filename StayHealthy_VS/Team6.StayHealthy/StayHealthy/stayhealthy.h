#pragma once

#include <QtWidgets/QMainWindow>
#include "qstackedwidget.h"
#include "qmessagebox.h"

class StayHealthy : public QWidget
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
    //Ui::StayHealthyClass ui;
    QStackedWidget* stack_;

    QWidget* SetupWelcomeUI();
    QWidget* SetupRegisterUI();
    QWidget* SetupLoginUI();
};
