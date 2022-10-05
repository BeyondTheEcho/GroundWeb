#pragma once
#include <string>
#include "ui_GroundWeb.h"

using namespace std;

class NetworkManager;

class CommandInputHandler
{
public:
	//Vars
	QString cmdString;

	//temp
	int tempPort;
	string ipString;

	//Instance References
	Ui::GroundWebClass ui;
	NetworkManager* network;

	//Command Handling
	void HandleCommand(std::string s);

	//Network Handling
	void InitNetwork();

	//Utility
	void StoreUIInst(Ui::GroundWebClass uiInst);
	void PrintToCMD(string s);
	string StringToLower(std::string str);
	bool validate_ip_segment(const std::string& str);
};

