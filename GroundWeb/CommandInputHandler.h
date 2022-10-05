#pragma once
#include <string>
#include "ui_GroundWeb.h"
#include "NetworkManager.h"

using namespace std;

class CommandInputHandler
{
public:
	//Vars
	QString cmdString;

	//Instance References
	Ui::GroundWebClass ui;
	//NetworkManager* network;

	//Command Handling
	void HandleCommand(std::string s);

	//Network Handling
	void InitNetwork();

	//Utility
	void StoreUIInst(Ui::GroundWebClass uiInst);
	void PrintToCMD(string s);
	string StringToLower(std::string str);
};

