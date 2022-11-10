#include "NetworkManager.h"

NetworkManager::NetworkManager(GroundWeb* web)
{
	UDPSocketIn = INVALID_SOCKET;
	UDPSocketOut = INVALID_SOCKET;
	TCPSocketIn = INVALID_SOCKET;
	TCPSocketOut = INVALID_SOCKET;

	UDPinAddr = { 0 };
	UDPoutAddr = { 0 };
	TCPinAddr = { 0 };
	TCPoutAddr = { 0 };

	m_GroundWeb = web;

	RegisterNetworkCommands();
	Init();
	CreateTCPSockets();
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::Init()
{
	m_GroundWeb->PrintToCMD("NetworkManager::Init() called.");

	WSADATA lpWSAData;
	int error = WSAStartup(MAKEWORD(2, 2), &lpWSAData);

	if (error != 0)
	{
		m_GroundWeb->PrintToCMD("WSAData failed with error code: " + error);
	}
}

void NetworkManager::CreateTCPSockets()
{
	m_GroundWeb->PrintToCMD("Creating TCP Sockets...");

	TCPSocketIn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPSocketIn == INVALID_SOCKET)
	{
		m_GroundWeb->PrintToCMD("ERROR: TCPSocketIn was not created.");
	}

	TCPSocketOut = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPSocketOut == INVALID_SOCKET)
	{
		m_GroundWeb->PrintToCMD("ERROR: TCPSocketOut was not created.");
		Shutdown();
	}
}

void NetworkManager::CreateUDPSockets()
{
	UDPSocketIn = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (UDPSocketIn == INVALID_SOCKET)
	{
		m_GroundWeb->PrintToCMD("ERROR: Failed to create UDPSocketIn");
		Shutdown();
	}

	UDPSocketOut = socket(AF_INET, SOCK_DGRAM, 0);

	if (UDPSocketOut == INVALID_SOCKET)
	{
		m_GroundWeb->PrintToCMD("ERROR: Failed to create UDPSocketOut");
		Shutdown();
	}
}

void NetworkManager::BindUDP()
{
	// Using IPv4
	UDPinAddr.sin_family = AF_INET;

	//Port
	UDPinAddr.sin_port = htons(stoi(m_Port));

	//From any available address (Computers can have multiple)
	UDPinAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindError = bind(UDPSocketIn, reinterpret_cast<SOCKADDR*>(&UDPinAddr), sizeof(UDPinAddr));

	if (bindError == SOCKET_ERROR)
	{
		m_GroundWeb->PrintToCMD("ERROR: Binding UDPSocketIn Failed");

		Shutdown();
	}
	else
	{
		m_GroundWeb->PrintToCMD("Bound UDPSocketIn");
	}
}

void NetworkManager::BindTCP()
{
	//IPv4
	TCPinAddr.sin_family = AF_INET;
	//Port to listen on
	TCPinAddr.sin_port = htons(stoi(m_Port));
	//Listen for any incoming connection
	TCPinAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindError = bind(TCPSocketIn, reinterpret_cast<sockaddr*>(&TCPinAddr), sizeof(TCPinAddr));

	if (bindError == SOCKET_ERROR)
	{
		m_GroundWeb->PrintToCMD("ERROR: Binding TCPSocketIn failed");
		Shutdown();
	}
	else
	{
		m_GroundWeb->PrintToCMD("Bound TCPSocketIn");
	}
}

void NetworkManager::ListenTCP()
{
	//MUST be called after binding TCP
	listen(TCPSocketIn, SOMAXCONN);

	m_GroundWeb->PrintToCMD("Listen called on TCPSocketIn");
}

void NetworkManager::ConnectTCP()
{
	//Client connection to the listening socket
	TCPoutAddr.sin_family = AF_INET;
	TCPoutAddr.sin_port = htons(8889);
	inet_pton(AF_INET, "127.0.0.1", &TCPoutAddr.sin_addr);

	int connectStatus = connect(TCPSocketOut, reinterpret_cast<sockaddr*>(&TCPoutAddr), sizeof(TCPoutAddr));

	if (connectStatus == SOCKET_ERROR)
	{
		m_GroundWeb->PrintToCMD("ERROR: Error connecting through TCP socket info supplied");
		Shutdown();
	}

	numConnections++;

	//Makes socket Async
	unsigned long bit = 1;
	ioctlsocket(TCPSocketOut, FIONBIO, &bit);
	ioctlsocket(TCPSocketIn, FIONBIO, &bit);
}

void NetworkManager::AcceptConnectionsTCP()
{
	m_GroundWeb->PrintToCMD("Now Accepting TCP Connections");

	while (true)
	{
		int clientSize = sizeof(TCPoutAddr);

		TCPSocketOut = accept(TCPSocketIn, reinterpret_cast<SOCKADDR*>(&TCPoutAddr), &clientSize);

		if (TCPSocketOut != INVALID_SOCKET)
		{
			numConnections++;

			char ipConnected[32];

			inet_ntop(AF_INET, &TCPoutAddr.sin_addr, ipConnected, 32);

			m_Clients.push_back(TCPSocketOut);
		}

		unsigned long bit = 1;
		ioctlsocket(TCPSocketIn, FIONBIO, &bit);
	}
}

void NetworkManager::SendDataTCP(const char* data)
{
	int totalByteSize = send(TCPSocketOut, data, strlen(data) + 1, 0);

	if (totalByteSize == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error = WSAEWOULDBLOCK)
		{
			string sData = data;

			m_GroundWeb->PrintToCMD("TCP - Sent the data across: " + sData + " size was: " + to_string(totalByteSize));
		}
		else
		{
			m_GroundWeb->PrintToCMD("ERROR: Failed to send TCP message");
			Shutdown(); //May need to be removed in the future
		}
	}
}

void NetworkManager::SetRemoteData()
{
	UDPoutAddr.sin_family = AF_INET;

	UDPoutAddr.sin_port = htons(8889);

	inet_pton(AF_INET, "127.0.0.1", &UDPoutAddr.sin_addr);
}

void NetworkManager::SendDataUDP(const char* data)
{
	int totalBytes = sendto(UDPSocketOut, data, strlen(data) + 1, 0, 
		reinterpret_cast<SOCKADDR*>(&UDPoutAddr), sizeof(UDPoutAddr));

	if (totalBytes == SOCKET_ERROR)
	{
		Shutdown();
	}

	m_GroundWeb->PrintToCMD("UDP - Sent: " + to_string(totalBytes) + " of data ");
}

int NetworkManager::ReceiveDataUDP(char* ReceiveBuffer)
{
	int BytesReceived = 0;
	int inAddrSize = sizeof(UDPinAddr);

	BytesReceived = recvfrom(UDPSocketIn, ReceiveBuffer, 65535, 0,
		reinterpret_cast<SOCKADDR*>(&UDPinAddr), &inAddrSize);

	if (BytesReceived == SOCKET_ERROR)
	{
		Shutdown();
	}

	return BytesReceived;
}

int NetworkManager::ReceiveDataTCP(char* message)
{
	int bytesReceived = recv(TCPSocketOut, message, strlen(message) + 1, 0);

	if(bytesReceived == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK)
		{
			m_GroundWeb->PrintToCMD("ERROR: Error receiving from TCP");
			Shutdown();
		}
	}

	return bytesReceived;
}

void NetworkManager::ReceiveMessage()
{
	while (true)
	{
		char rcvMessage[MAX_RCV_SIZE];
		int size = ReceiveDataTCP(rcvMessage);
		if (size > 0)
		{
			string msg = rcvMessage;
			m_GroundWeb->PrintToCMD("Received Message: " + msg);
		}
	}
}

void NetworkManager::Shutdown()
{
	m_GroundWeb->PrintToCMD("NetworkManager::Shutdown() called.");

	int errorCode = WSAGetLastError();
	if (errorCode != 0)
	{
		m_GroundWeb->PrintToCMD("Forced shutdown due to WSAError#: " + errorCode);
	}

	if (UDPSocketIn != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketIn) != 0)
		{
			m_GroundWeb->PrintToCMD("ERROR: error closing UDP In Socket.");
		}
	}

	if (UDPSocketOut != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketIn) != 0)
		{
			m_GroundWeb->PrintToCMD("ERROR: error closing UDP Out Socket.");
		}
	}

	if (TCPSocketIn != INVALID_SOCKET)
	{
		if (closesocket(TCPSocketIn) != 0)
		{
			m_GroundWeb->PrintToCMD("ERROR: Failed to close TCPSocketIn");
		}
	}

	if (TCPSocketOut != INVALID_SOCKET)
	{
		if (closesocket(TCPSocketOut) != 0)
		{
			m_GroundWeb->PrintToCMD("ERROR: Failed to close TCPSocketOut");
		}
	}

	WSACleanup();
	exit(0);
}

//Register Commands

void NetworkManager::RegisterNetworkCommands()
{
	string flagServerDesc = "Sets a flag to allow you to act as a server";
	m_GroundWeb->RegisterCommand("flagserver", flagServerDesc, [this](std::string _)
		{
			FlagForServer();
		});

	string flagClientDesc = "Sets a flag to allow you to act as a client";
	m_GroundWeb->RegisterCommand("flagclient", flagServerDesc, [this](std::string _)
		{
			FlagForClient();
		});

	string startNetworkingDesc = "Starts the network if all valid information is populated";
	m_GroundWeb->RegisterCommand("startnetwork", startNetworkingDesc, [this](std::string _)
		{
			StartNetworking();
		});

	string setIPDesc = "Sets the ip to be used for a connection";
	m_GroundWeb->RegisterCommand("setip", setIPDesc, [this](std::string enteredIP)
		{
			SetIP(enteredIP);
		});

	string setPortDesc = "Sets the port to be used for a connection";
	m_GroundWeb->RegisterCommand("setport", setPortDesc, [this](std::string enteredPort)
		{
			SetPort(enteredPort);
		});

	string printNetworkInfoDesc = "Sets the port to be used for a connection";
	m_GroundWeb->RegisterCommand("networksettings", printNetworkInfoDesc, [this](std::string _)
		{
			PrintNetworkSettings();
		});

	m_GroundWeb->PrintToCMD("Network commands registered");
}

//Network Commands
void NetworkManager::StartNetworking()
{
	if (m_IsIPSet == false)
	{
		m_GroundWeb->PrintToCMD("ERROR: IP Address not set");
		return;
	}

	if (m_IsServer == true)
	{
		StartServer();
	}
	else if (m_IsServer == false)
	{
		StartClient();
	}
}

void NetworkManager::StartServer()
{
	m_GroundWeb->PrintToCMD("Staring up as server...");

	BindTCP();
	ListenTCP();

	m_ListenThread = thread([this]
		{
			AcceptConnectionsTCP();
		});
	while (true)
	{
		if (numConnections > 0)
		{
			m_GroundWeb->PrintToCMD("ReceiveMessage() Called");

			m_ReceiveThread = thread([this]
				{
					ReceiveMessage();
				});

			break;
		}
	}
}

void NetworkManager::StartClient()
{
	m_GroundWeb->PrintToCMD("Staring up as client...");
}

void NetworkManager::FlagForServer()
{
	if (m_IsServer == false)
	{
		m_IsServer = !m_IsServer;
	}

	m_GroundWeb->PrintToCMD("You are now flagged to act as a server.");
}

void NetworkManager::FlagForClient()
{
	if (m_IsServer == true)
	{
		m_IsServer = !m_IsServer;
	}

	m_GroundWeb->PrintToCMD("You are now flagged to act as a client");
}

void NetworkManager::PrintNetworkSettings()
{
	if (m_IsServer == true)
	{
		m_GroundWeb->PrintToCMD("Mode: Server");
	}
	else
	{
		m_GroundWeb->PrintToCMD("Mode: Client");
	}

	m_GroundWeb->PrintToCMD("Port: " + m_Port);
	m_GroundWeb->PrintToCMD("IP: " + m_IP);
}

void NetworkManager::SetIP(string ip)
{
	m_IP = ip;
	m_IsIPSet = true;

	m_GroundWeb->PrintToCMD("IP Address set to: " + ip);
}

void NetworkManager::SetPort(string port)
{
	m_GroundWeb->PrintToCMD("Default Port: 8889");
	m_GroundWeb->PrintToCMD("Current Port: " + m_Port);

	if (m_PortOverride == false)
	{
		m_GroundWeb->PrintToCMD("Default port 8889 is recommended, to override the default call /setport again");
		m_PortOverride = true;
	}
	else
	{
		m_Port = port;
		m_GroundWeb->PrintToCMD("Port set to: " + m_Port);
	}
}