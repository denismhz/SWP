#include "tests_stacked.h"
#include "main_menu.h"

TestsStackedClass::TestsStackedClass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
  MainMenu *m = new MainMenu();
        ui.stackedWidget->addWidget(m);
  connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(b_clicke()));
  connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(a_clicke()));
          
}

TestsStackedClass::~TestsStackedClass()
{
}

void TestsStackedClass::b_clicke() {
  ui.stackedWidget->setCurrentIndex(ui.stackedWidget->currentIndex()+1);
  qDebug() << ui.stackedWidget->currentIndex();
}

void TestsStackedClass::a_clicke() {
  ui.stackedWidget->setCurrentIndex(1);
  qDebug() << "asdasd";
}
