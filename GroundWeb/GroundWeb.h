#pragma once
#include <string>
#include <QtWidgets/QWidget>
#include <regex>
#include <vector>
#include <functional>

#include "ui_GroundWeb.h"
#include "qstring.h"

using std::string;
using std::vector;

class Command
{
public:
    typedef std::function<void(std::string)> CommandHandler;
    string m_Name;
    string m_Description;
    CommandHandler m_Handler;

    Command(string name, string desciption, CommandHandler handler)
        : m_Name(name)
        , m_Description(desciption)
        , m_Handler(handler)
    {

    }
};

class GroundWeb : public QWidget
{
    Q_OBJECT

public slots:
    void on_userInputField_returnPressed();

public:
    GroundWeb(QWidget *parent = nullptr);
    ~GroundWeb();
	void PrintToCMD(string s);
    void RegisterCommand(string name, string description, Command::CommandHandler handler);

private:
    //Vars
    string m_CommandInputString;
    vector<Command> m_Commands;

    //Functions
    void HandleCommands(string cmdString);
    void RegisterBuiltInCommands();

    //Command Line Functions
    void ClearCommandLine();
    void HelpCommandLine();

    //Utility Funcs
    string StringToLower(string str);
    bool StringStartsWith(string str, string substr);

    Ui::GroundWebClass m_UI;
};

