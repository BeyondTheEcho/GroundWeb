#pragma once
#include "NetworkManager.h"

#define CHAR_SIZE 30;

class HangManModule
{
public:
	GroundWeb* m_GroundWeb;
	NetworkManager* m_NetworkManager;

	HangManModule(GroundWeb* g, NetworkManager* n);
	~HangManModule();

	void RegisterCommands();
	void Wizard();
	void UpdateChosenWord(string word);
	void PopulateBlankedArray();
	void Guess(string guess);
	void SendGameState();
	void ReceiveGameState(NetworkManager::NetworkData data);

	NetworkManager::NetworkData m_HangManData;

	//Vars
	bool m_IsWizard = false;
	char m_ChosenWord[30];
	char m_BlankedWord[30];
};