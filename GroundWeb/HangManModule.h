#pragma once
#include "GroundWeb.h"

class HangManModule
{
public:
	GroundWeb *m_GroundWeb;

	HangManModule(GroundWeb *g);
	~HangManModule();

	void RegisterCommands();
	void DoThing(string s);
};

