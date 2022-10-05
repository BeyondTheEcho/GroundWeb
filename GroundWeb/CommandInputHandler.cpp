#include <algorithm>
#include <regex>
#include "CommandInputHandler.h"
#include <iostream>
#include "ui_GroundWeb.h"
#include "NetworkManager.h"

using namespace std;

void CommandInputHandler::HandleCommand(string s)
{
	//Take string and converts to lower to manage input
	string formatedString = StringToLower(s);

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
			connectionPort = std::stoi(regexOutput[1]);
			PrintToCMD("Port Number Set To: " + std::to_string(connectionPort));
			portSelected = true;
		}
	}
	else if (regex_search(s, regexOutput, ipPattern))
	{
		if (validate_ip_segment(regexOutput[2]) &&
			validate_ip_segment(regexOutput[3]) &&
			validate_ip_segment(regexOutput[4]) &&
			validate_ip_segment(regexOutput[5]))
		{
			ipString = regexOutput[1];
			PrintToCMD("Ip Set To: " + ipString);
			ipSelected = true;
		}
		else
		{
			PrintToCMD("Invalid IP Address Entered");
		}
	}
	else if (s == "help")
	{
		PrintHelp();
	}
	else if (s == "clr")
	{
		ClearConsole();
	}
	else if (s == "config")
	{
		CheckNetworkConfig();
	}
	else if (s == "init")
	{
		InitNetwork();
	}
	else if (s == "quit")
	{
		network->ShutdownApplication();
	}
	else if (networkInitialized == true)
	{
		PrintToCMD(s);
		network->SendData(s.c_str());
	}
	else
	{
		PrintToCMD(s);
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
	if (portSelected == true && ipSelected == true)
	{
		network = NetworkManager::GetInstance();
		network->Init(this);
		network->CreateUDPSockets();
		network->BindUDP();
		network->SetRemoteData(connectionPort, ipString);
		network->StartMultithreading();

		networkInitialized = true;

		PrintToCMD("Network Connection Initialized");
	}
	else
	{
		PrintToCMD("ERROR: Either your port or ip are not set");
		PrintToCMD("Type -p \"X.X.X.X\" where x represents the port you wish to use");
		PrintToCMD("Type -ip and enter your up in quotations. Example -ip \"192.168.2.1\"");
	}
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

void CommandInputHandler::PrintHelp()
{
	PrintToCMD("Help - Enabled this menu");
	PrintToCMD("CLR - Clear Console History");
	PrintToCMD("Config - Prints Network Status");
	PrintToCMD("Init - Initializes Network Connection Once All Required Field Are Entered");
}

void CommandInputHandler::ClearConsole()
{
	cmdString.clear();
	ui.consoleOutput->setText(cmdString);
}

void CommandInputHandler::CheckNetworkConfig()
{
	PrintToCMD("Port Selected: " + portSelected);
	PrintToCMD("IP Entered: " + ipSelected);
	PrintToCMD("Network Initialized: " + networkInitialized);
}