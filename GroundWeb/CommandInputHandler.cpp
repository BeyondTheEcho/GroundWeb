#include <algorithm>
#include "CommandInputHandler.h"
#include "ui_GroundWeb.h"
#include "NetworkManager.h"


using namespace std;

void CommandInputHandler::HandleCommand(string s)
{
	//Take string and converts to lower to manage input
	string formatedString = StringToLower(s);

	if (s == "help")
	{
		PrintToCMD("'CLR' Clears Console");
		PrintToCMD("This will be a help string");
	}
	if (s == "clr")
	{
		cmdString.clear();
		ui.consoleOutput->setText(cmdString);
	}
}

string CommandInputHandler::StringToLower(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

void CommandInputHandler::StoreUIInst(Ui::GroundWebClass uiInst)
{
	ui = uiInst;
}

void CommandInputHandler::PrintToCMD(string s)
{
	s.append("\n");
	cmdString.append(QString::fromStdString(s));

	ui.consoleOutput->setText(cmdString);
}


void CommandInputHandler::InitNetwork()
{
	//network = NetworkManager::GetInstance();
	//network->Init();
}
