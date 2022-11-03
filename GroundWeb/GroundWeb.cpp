#include "GroundWeb.h"

GroundWeb::GroundWeb(QWidget *parent) : QWidget(parent)
{
	//Execute code on setup here
    ui.setupUi(this);

    string helpDesc = "Displays a list of all registered commands";
    RegisterCommand("help", helpDesc, [this](std::string content)
        {
            string printStr = "==== Commands List ====\n";        

            for (auto command : commands)
            {
                printStr.append("/" + command.m_Name + " - " + command.m_Description + "\n");
            }

            PrintToCMD(printStr);
        });

    ui.consoleOutput->setText("Type help for a list of commands");
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

void GroundWeb::RegisterCommand(string name, string description, Command::CommandHandler handler)
{
    commands.emplace_back(name, description, handler);
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
        if (StringStartsWith(cmdString, command.m_Name))
        {
            string content = cmdString.substr(command.m_Name.length());       
            command.m_Handler(content);
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
