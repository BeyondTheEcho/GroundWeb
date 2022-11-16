#include "GroundWeb.h"

GroundWeb::GroundWeb(QWidget *parent) : QWidget(parent)
{
	//Execute code on setup here
    m_UI.setupUi(this);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Active, QPalette::Window, QColor(1, 4, 9));
    p.setColor(QPalette::Active, QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::Active, QPalette::Highlight, QColor(65, 138, 201));
    p.setColor(QPalette::Active, QPalette::ButtonText, QColor(255, 255, 255));
    p.setColor(QPalette::Active, QPalette::WindowText, QColor(255, 255, 255));
    p.setColor(QPalette::Active, QPalette::Base, QColor(13, 17, 23));
    p.setColor(QPalette::Active, QPalette::Text, QColor(201, 209, 217));
    p.setColor(QPalette::Inactive, QPalette::Window, QColor(1, 4, 9));
    p.setColor(QPalette::Inactive, QPalette::Button, QColor(53, 53, 53));
    p.setColor(QPalette::Inactive, QPalette::Highlight, QColor(65, 138, 201));
    p.setColor(QPalette::Inactive, QPalette::ButtonText, QColor(255, 255, 255));
    p.setColor(QPalette::Inactive, QPalette::WindowText, QColor(255, 255, 255));
    p.setColor(QPalette::Inactive, QPalette::Base, QColor(13, 17, 23));
    p.setColor(QPalette::Inactive, QPalette::Text, QColor(201, 209, 217));
    qApp->setPalette(p);

    p.setColor(QPalette::Window, QColor(13, 17, 23));
    m_UI.scrollArea->setPalette(p);
    m_UI.label->setPalette(p);

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

    m_UI.scrollArea->verticalScrollBar()->setMaximum(INT32_MAX);
    m_UI.scrollArea->verticalScrollBar()->setValue(m_UI.scrollArea->verticalScrollBar()->maximum());
}

void GroundWeb::PrintToCMD(string s)
{
	QString cmdString = m_UI.label->text();

	cmdString.append("<br/>");
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

            string modifiedContent = content;
            modifiedContent.erase(0, 1);

            command.m_Handler(modifiedContent);
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

    PrintToCMD("Builtin commands registered");
}
