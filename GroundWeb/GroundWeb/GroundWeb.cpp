#include "GroundWeb.h"
#include <ui_GroundWeb.h>

GroundWeb::GroundWeb(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

GroundWeb::~GroundWeb()
{}

void GroundWeb::on_userInputField_returnPressed()
{
    ui.consoleOutput->setText("Test");
}
