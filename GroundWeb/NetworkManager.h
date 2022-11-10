#pragma once
#pragma comment (lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include  <WS2tcpip.h>
#include <thread>


#include "iostream"
#include "GroundWeb.h"

using std::to_string;
using std::thread;
using std::stoi;

class NetworkManager
{
public:
	NetworkManager(GroundWeb* web);
	~NetworkManager();

	static const int MAX_RCV_SIZE = 65535;

	//Socket Creation
	void CreateUDPSockets();
	void CreateTCPSockets();
	//Bind Sockets
	void BindUDP();
	void BindTCP();
	//Listen
	void ListenTCP();
	//Client Side Connection
	void ConnectTCP();
	//Server Side Accept Connections
	void AcceptConnectionsTCP();
	//Send Data
	void SendDataTCP(const char* data);
	void SendDataUDP(const char* data);
	//Receive Data
	int ReceiveDataUDP(char* ReceiveBuffer);
	int ReceiveDataTCP(char* message);
	//Receive Message
	void ReceiveMessage();

	void SetRemoteData();
	int GetNumConnections() { return numConnections; }

	//Commands
	void RegisterNetworkCommands();
	void StartNetworking();
	void StartServer();
	void StartClient();
	void FlagForServer();
	void FlagForClient();
	void PrintNetworkSettings();
	void SetIP(string ip);
	void SetPort(string port);
	void SendMessageTCP(string message);

	//Call on Shutdown
	void Shutdown();

private:
	//Private Functions
	void Init();

	//Private Types
	GroundWeb* m_GroundWeb;

	SOCKET UDPSocketIn;
	SOCKET UDPSocketOut;
	SOCKET TCPSocketIn;
	SOCKET TCPSocketOut;

	SOCKADDR_IN UDPoutAddr;
	SOCKADDR_IN UDPinAddr;
	SOCKADDR_IN TCPinAddr;
	SOCKADDR_IN TCPoutAddr;

	//Private Variables
	int numConnections = 0;
	bool m_IsServer = false;
	bool m_IsIPSet = false;
	bool m_IsConnected = false;
	bool m_PortOverride = false;
	string m_IP;
	string m_Port = "8889";
	vector<SOCKET> m_Clients;

	//Private Threads
	thread m_ListenThread;
	thread m_ReceiveThread;
};

