#include "main_menu.h"

MainMenu::MainMenu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.homeButton, SIGNAL(clicked()), parentWidget(), SLOT(HomePressed()));
}

MainMenu::~MainMenu()
{
}