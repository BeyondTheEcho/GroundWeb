#include "GroundWeb.h"
#include <ui_GroundWeb.h>
#include <string>

using namespace std;

string commandInputString;

//Vars
CommandInputHandler* cmd;

GroundWeb::GroundWeb(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    cmd = new CommandInputHandler();
    cmd->StoreUIInst(ui);
}

GroundWeb::~GroundWeb()
{}

void GroundWeb::on_userInputField_returnPressed()
{
    //Take in the text from inputfield as Qstring and casts to std::string
    commandInputString = ui.userInputField->text().toUtf8().constData();

    //Passes converted string to command handler
    cmd->HandleCommand(commandInputString);

    //Cleanup cmdstring and input field
    commandInputString.clear();
    ui.userInputField->clear();
}
