#include "GroundWeb.h"

GroundWeb::GroundWeb(QWidget *parent) : QWidget(parent)
{
	//Execute code on setup here
    m_UI.setupUi(this);

    RegisterBuiltInCommands();

    ClearCommandLine();
    PrintToCMD("Type help for a list of commands");
}

GroundWeb::~GroundWeb()
{}

void GroundWeb::on_userInputField_returnPressed()
{
    //Take in the text from inputfield as Qstring and casts to std::string
    m_CommandInputString = m_UI.userInputField->text().toUtf8().constData();

    //Prints user entered text to the cmd
    PrintToCMD(m_CommandInputString);

    //Passes converted string to command handler
    HandleCommands(m_CommandInputString);

    //Cleanup cmdstring and input field
    m_CommandInputString.clear();
    m_UI.userInputField->clear();
}

void GroundWeb::PrintToCMD(string s)
{
	QString cmdString = m_UI.label->text();

	cmdString.append("\n");
	cmdString.append(QString::fromStdString(s));

	m_UI.label->setText(cmdString);
}

void GroundWeb::RegisterCommand(string name, string description, Command::CommandHandler handler)
{
    m_Commands.emplace_back(name, description, handler);
}

void GroundWeb::HandleCommands(string cmdString)
{
    if (cmdString[0] != '/')
    {
        return;
    }
    
    cmdString = cmdString.substr(1);

    for (auto command : m_Commands)
    {
        if (StringStartsWith(cmdString, command.m_Name))
        {
            string content = cmdString.substr(command.m_Name.length());       
            command.m_Handler(content);
        }
    }
}

// Utility Functions

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

// Command Line Functions

void GroundWeb::ClearCommandLine()
{
    m_UI.label->clear();
}

void GroundWeb::HelpCommandLine()
{
    string printStr = "==== Commands List ====\n";

    for (auto command : m_Commands)
    {
        printStr.append("/" + command.m_Name + " - " + command.m_Description + "\n");
    }

    PrintToCMD(printStr);
}

// Registered Commands

void GroundWeb::RegisterBuiltInCommands()
{
    string helpDesc = "Displays a list of all registered commands";
    RegisterCommand("help", helpDesc, [this](std::string _)
        {
            HelpCommandLine();
        });

    string clearDesc = "Clears all command line history";
    RegisterCommand("clear", clearDesc, [this](std::string _)
        {
            ClearCommandLine();
        });
}
