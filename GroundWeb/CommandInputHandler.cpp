#include <algorithm>
#include <regex>
#include "CommandInputHandler.h"
#include <iostream>
#include "ui_GroundWeb.h"
#include "NetworkManager.h"

using namespace std;

void CommandInputHandler::HandleCommand(string s)
{
	//Regex Patterns
	regex portPattern("-p \"(\\d+)\"");
	regex ipPattern("-ip \"((\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3}))\"");

	smatch regexOutput;

	if (regex_search(s, regexOutput, portPattern))
	{
		string port = regexOutput[1];

		if (port.length() > 5)
		{
			PrintToCMD("Invalid Port Number");
		}
		else
		{
			tempPort = std::stoi(regexOutput[1]);
			PrintToCMD("Port Number Set To: " + std::to_string(tempPort));
		}
	}

	if (regex_search(s, regexOutput, ipPattern))
	{
		if (validate_ip_segment(regexOutput[2]) &&
			validate_ip_segment(regexOutput[3]) &&
			validate_ip_segment(regexOutput[4]) &&
			validate_ip_segment(regexOutput[5])
			)
		{
			ipString = regexOutput[1];
			PrintToCMD("Ip Set To: " + ipString);
		}
		else
		{
			PrintToCMD("Invalid IP Address Entered");
		}
	}


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
	if (s == "init")
	{
		InitNetwork();
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
	network = NetworkManager::GetInstance();
	network->Init(this);
}

bool CommandInputHandler::validate_ip_segment(const std::string& str)
{
	try
	{
		const int result = std::stoi(str);
		if (result < 0) return false;
		if (result > 255) return false;
		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}
