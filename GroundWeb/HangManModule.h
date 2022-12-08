#pragma once
#include "NetworkManager.h"

class HangManModule
{
public:
	NetworkManager *m_Network;

	HangManModule(NetworkManager *n);
	~HangManModule();
};

