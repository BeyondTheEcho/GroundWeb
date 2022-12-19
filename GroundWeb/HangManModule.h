#pragma once
#include "NetworkManager.h"

class HangManModule
{
public:
	GroundWeb* m_GroundWeb;
	NetworkManager* m_NetworkManager;

	HangManModule(GroundWeb* g, NetworkManager* n);
	~HangManModule();

	void RegisterCommands();
	void DoThing(string s);
};

