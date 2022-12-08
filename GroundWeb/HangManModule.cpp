#include "HangManModule.h"

HangManModule::HangManModule(GroundWeb *g)
{
	m_GroundWeb = g;
    RegisterCommands();
}

HangManModule::~HangManModule()
{

}

void HangManModule::RegisterCommands()
{
    string DoThingDesc = "This does a thing";
    m_GroundWeb->RegisterCommand("dothing", DoThingDesc, [this](std::string userInput)
        {
            DoThing(userInput);
        });


}

void HangManModule::DoThing(string s)
{
    m_GroundWeb->PrintToCMD(s);
}

