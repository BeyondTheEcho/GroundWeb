#pragma once
#pragma comment (lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <string>
#include <thread>
#include <windows.h>
#include <WinSock2.h>

using namespace std;

class CommandInputHandler;

class NetworkManager
{
public:
	//Instances
	CommandInputHandler* cmd;

	//Threads
	thread listenThread;

	//Vars
	int userPort;
	string connectIP;
	string userName;
	bool isThreadRunning = true;
	char* recString = new char[65543];


	static NetworkManager* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new NetworkManager();
		}

		return instance;
	}

	void Init(CommandInputHandler* cmdInst);
	void Shutdown();
	void CreateUDPSockets();
	void BindUDP();
	void SetRemoteData(int port, string cxIP);
	void SendData(const char* data);
	int ReceiveData(char* ReceiveBuffer);
	void ListenForMessage();
	void ShutdownApplication();
	void StartMultithreading();

private:

	//Sockets
	SOCKET UDPSocketIn;
	SOCKET UDPSocketOut;

	SOCKADDR_IN outAddr;
	SOCKADDR_IN inAddr;

	//Static Ref
	static NetworkManager* instance;
};

