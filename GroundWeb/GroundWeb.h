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
    string name;
    CommandHandler handler;

    Command(string cmdName, CommandHandler cmdHandler)
        : name(cmdName)
        , handler(cmdHandler)
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
    void RegisterCommand(string name, Command::CommandHandler handler);

private:
    //Vars
    string commandInputString;
    vector<Command> commands;

    //Functions
    void HandleCommands(string cmdString);

    //Utility Funcs
    string StringToLower(string str);
    bool StringStartsWith(string str, string substr);

    Ui::GroundWebClass ui;
};

