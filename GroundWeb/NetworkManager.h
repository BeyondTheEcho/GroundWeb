#pragma once
#pragma comment (lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSock2.h>
#include  <WS2tcpip.h>
#include <thread>
#include <mutex>
#include "iostream"
#include "GroundWeb.h"

class HangManModule;

using std::to_string;
using std::thread;
using std::stoi;
using std::mutex;

class NetworkManager
{
public:
	NetworkManager(GroundWeb* web);
	~NetworkManager();

	//Struct
	struct NetworkData
	{
		char m_Message[1000];
		char m_ChosenWord[30];
		char m_BlankedWord[30];
	};

	static constexpr int MAX_RCV_SIZE = 65535;
	
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
	void ConnectTCP(string ip);
	//Server Side Accept Connections
	void AcceptConnectionsTCP();
	//Send Data
	void SendDataTCP(const char* data);
	void SendDataTCPServer(const char* data);
	void SendDataUDP(const char* data);
	//Send Hangman Data
	void SendHangManDataTCP(NetworkData* net);
	void SendHangManDataTCPServer(NetworkData* net);
	//Receive Data
	int ReceiveDataUDP(char* ReceiveBuffer);
	int ReceiveDataTCP(char* message, SOCKET sock);
	//Receive Message
	void ReceiveMessageClient();
	void ReceiveMessageServer();
	void SpinReceiveMessageThread();

	void RelayToCMD(string s);
	void PopulateHangManInstance(HangManModule* h);

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
	void SetUsername(string message);
	void SetMessageColor(string message);
	void SendMessageTest(const char* data);
	void InitClient();

	//Public Vars
	bool m_IsServer = false;
	bool m_IsConnected = false;
	string FormatUserMessage(string message);
	string FormatServerMessage(string message);

	//Call on Shutdown
	void Shutdown();

private:
	//Private Functions
	void Init();

	//Private Types
	GroundWeb* m_GroundWeb;
	HangManModule* m_HangMan;

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
	bool m_IsIPSet = false;
	bool m_PortOverride = false;
	bool m_ListenThreadIsRunning = false;
	bool m_ReceiveThreadIsRunning = false;
	bool m_UsernameSet = false;
	string m_IP;
	string m_Port = "8889";
	string m_Username;
	string m_Servername = "SERVER";
	vector<SOCKET> m_Clients;

	//User Hex Colors
	string m_UsernameColor = "#2bff56";
	string m_MessageColor = "#ffffff";
	string m_MessageColorDefault = "#ffffff";
	string m_MessageColorBlue = "#3633ff";
	string m_MessageColorRed = "#ff0000";
	string m_MessageColorYellow = "#ffdd00";

	//Server Hex Colors
	string m_ServerUsernameColor = "#1f9eff";
	string m_ServerMessageColor = "#6b3ede";

	//Private Threads
	thread m_ListenThread;
	thread m_ReceiveThread;

	//Mutex
	mutex m_Mutex1;
};