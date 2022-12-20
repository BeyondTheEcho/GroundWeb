#include "HangManModule.h"

HangManModule::HangManModule(GroundWeb* g, NetworkManager* n)
{
	m_GroundWeb = g;
    m_NetworkManager = n;
    RegisterCommands();
    m_NetworkManager->PopulateHangManInstance(this);
}

HangManModule::~HangManModule()
{

}

void HangManModule::RegisterCommands()
{
    string wizardDesc = "Identifies your role as Wizard";
    m_GroundWeb->RegisterCommand("wizard", wizardDesc, [this](std::string _)
        {
            Wizard();
        });

    string chooseWordDesc = "Enters your chosen word for hangman";
    m_GroundWeb->RegisterCommand("chooseword", chooseWordDesc, [this](std::string word)
        {
            UpdateChosenWord(word);
        });

    string guessDesc = "Enters a guess";
    m_GroundWeb->RegisterCommand("guess", guessDesc, [this](std::string guess)
        {
            Guess(guess);
        });
}

void HangManModule::Wizard()
{
    m_IsWizard = true;
    m_GroundWeb->PrintToCMD("You are now the wizard");
}

void HangManModule::UpdateChosenWord(string word)
{
    if (m_IsWizard == true)
    {
        strncpy(m_ChosenWord, word.c_str(), sizeof(m_ChosenWord));
        PopulateBlankedArray();
    }
}

void HangManModule::PopulateBlankedArray()
{
    int i = 0;

    while (true)
    {
	    if (m_ChosenWord[i] != '\0')
	    {
            m_BlankedWord[i] = '#';
	    }
        else
        {
            m_BlankedWord[i] = '\0';
            break;
        }

        i++;
    }

    SendGameState();
}

void HangManModule::Guess(string guess)
{
    int i = 0;
    const char* guessedChar = guess.c_str();

    while (true)
    {
        if (m_ChosenWord[i] == '\0')
        {
            break;
        }

        if (m_ChosenWord[i] == guessedChar[0])
        {
            m_BlankedWord[i] = m_ChosenWord[i];
        }

        i++;
    }

    SendGameState();

    string msg = m_BlankedWord;
    m_GroundWeb->PrintToCMD(msg);
}

void HangManModule::SendGameState()
{
    string dataType = "hangman";
    
	strncpy(m_HangManData.m_Message, dataType.c_str(), sizeof(m_HangManData.m_Message));
    strncpy(m_HangManData.m_BlankedWord, m_BlankedWord, sizeof(m_HangManData.m_BlankedWord));
    strncpy(m_HangManData.m_ChosenWord, m_ChosenWord, sizeof(m_HangManData.m_ChosenWord));

    if (m_NetworkManager->m_IsConnected)
    {
        if (m_NetworkManager->m_IsServer)
        {
            m_NetworkManager->SendHangManDataTCPServer(&m_HangManData);
        }
        else
        {
            m_NetworkManager->SendHangManDataTCP(&m_HangManData);
        }
    }
    else
    {
        m_GroundWeb->PrintToCMD("You Are Not Connected");
    }
}

void HangManModule::ReceiveGameState(NetworkManager::NetworkData data)
{
	string dataType = "hangman";
    
	strncpy(m_HangManData.m_Message, dataType.c_str(), sizeof(m_HangManData.m_Message));
    strncpy(m_HangManData.m_BlankedWord, m_BlankedWord, sizeof(m_HangManData.m_BlankedWord));
    strncpy(m_HangManData.m_ChosenWord, m_ChosenWord, sizeof(m_HangManData.m_ChosenWord));

    string msg = m_BlankedWord;
    m_GroundWeb->PrintToCMD(msg);
}

