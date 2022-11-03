#include "GroundWeb.h"

GroundWeb::GroundWeb(QWidget *parent) : QWidget(parent)
{
	//Execute code on setup here
    ui.setupUi(this);

    RegisterCommand("help", [this](std::string content)
        {
            string printStr = "==== Commands List ====\n";        

            for (auto command : commands)
            {
                printStr.append(command.name + "\n");
            }

            PrintToCMD(printStr);
        });

    ui.consoleOutput->setText("Type Help For A List Of Commands");
}

GroundWeb::~GroundWeb()
{}

void GroundWeb::on_userInputField_returnPressed()
{
    //Take in the text from inputfield as Qstring and casts to std::string
    commandInputString = ui.userInputField->text().toUtf8().constData();

    //Passes converted string to command handler
    HandleCommands(commandInputString);

    //Cleanup cmdstring and input field
    commandInputString.clear();
    ui.userInputField->clear();
}

void GroundWeb::PrintToCMD(string s)
{
	QString cmdString;

	s.append("\n");
	cmdString.append(QString::fromStdString(s));

	ui.consoleOutput->setText(cmdString);
}

void GroundWeb::RegisterCommand(string name, Command::CommandHandler handler)
{
    commands.emplace_back(name, handler);
}

void GroundWeb::HandleCommands(string cmdString)
{
    if (cmdString[0] != '/')
    {
        return;
    }
    
    cmdString = cmdString.substr(1);

    for (auto command : commands)
    {
        if (StringStartsWith(cmdString, command.name))
        {
            string content = cmdString.substr(command.name.length());       
            command.handler(content);
        }
    }
}

//Utility Functions

string GroundWeb::StringToLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}

bool GroundWeb::StringStartsWith(string str, string substr)
{
    int length = substr.length();
    string start = str.substr(0, length);

    return start == substr;
}
