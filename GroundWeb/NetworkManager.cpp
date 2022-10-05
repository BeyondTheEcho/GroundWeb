#include "NetworkManager.h"
#include "iostream"
#include  <WS2tcpip.h>
#include <thread>
#include "CommandInputHandler.h"

using namespace std;

NetworkManager* NetworkManager::instance = nullptr;

void NetworkManager::Init(CommandInputHandler* cmdInst)
{
	cmd = cmdInst;

	cmd->PrintToCMD("NetworkManager::Init() called.");

	WSADATA lpWSAData;
	int error = WSAStartup(MAKEWORD(2, 2), &lpWSAData);

	if (error != 0)
	{
		cmd->PrintToCMD("WSAData failed with error code: " + error);
	}
}

void NetworkManager::CreateUDPSockets()
{
	u_long isBlocking = 1;

	UDPSocketIn = socket(AF_INET, SOCK_DGRAM, 0);

	ioctlsocket(UDPSocketIn, FIONBIO, &isBlocking);

	if (UDPSocketIn == INVALID_SOCKET)
	{
		cmd->PrintToCMD("Failed to create inbound socket");
		Shutdown();
	}

	UDPSocketOut = socket(AF_INET, SOCK_DGRAM, 0);

	if (UDPSocketOut == INVALID_SOCKET)
	{
		cmd->PrintToCMD("Failed to create outbound socket");
		Shutdown();
	}
}

void NetworkManager::SetRemoteData(int port, string cxIP)
{
	outAddr.sin_family = AF_INET;

	outAddr.sin_port = htons(port);

	inet_pton(AF_INET, cxIP.c_str(), &outAddr.sin_addr);
}

void NetworkManager::StartMultithreading()
{
	listenThread = thread([this]
	{
		ListenForMessage();
	});
}

void NetworkManager::ListenForMessage()
{
	while (isThreadRunning)
	{
		int rcvSize = ReceiveData(recString);

		if (rcvSize > 0)
		{
			cmd->PrintToCMD(recString);
		}
	}
}

int NetworkManager::ReceiveData(char* ReceiveBuffer)
{
	int BytesReceived = 0;
	int inAddrSize = sizeof(inAddr);

	BytesReceived = recvfrom(UDPSocketIn, ReceiveBuffer, 65535, 0,
		reinterpret_cast<SOCKADDR*>(&inAddr), &inAddrSize);

	if (BytesReceived == WSAEWOULDBLOCK)
	{
		Shutdown();
	}

	return BytesReceived;
}

void NetworkManager::BindUDP()
{
	// Using IPv4
	inAddr.sin_family = AF_INET;

	//Port 8889
	inAddr.sin_port = htons(8889);

	//From any available address (Computers can have multiple)
	inAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindError = ::bind(UDPSocketIn, reinterpret_cast<SOCKADDR*>(&inAddr), sizeof(inAddr));

	if (bindError == SOCKET_ERROR)
	{
		cmd->PrintToCMD("[ERROR] binding failed.");

		Shutdown();
	}
}

void NetworkManager::SendData(const char* data)
{
	int totalBytes = sendto(UDPSocketOut, data, strlen(data) + 1, 0, 
		reinterpret_cast<SOCKADDR*>(&outAddr), sizeof(outAddr));

	if (totalBytes == SOCKET_ERROR)
	{
		Shutdown();
	}

	cmd->PrintToCMD("sent : " + to_string(totalBytes) + " of data");
}

void NetworkManager::Shutdown()
{
	cmd->PrintToCMD("NetworkManager::Shutdown() called.");

	int errorCode = WSAGetLastError();
	if (errorCode != 0)
	{
		cmd->PrintToCMD("Forced shutdown due to WSAError#: " + to_string(errorCode));
	}

	if (UDPSocketIn != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketIn) != 0)
		{
			cmd->PrintToCMD("[ERROR] error closing UDP In Socket.");
		}
	}

	if (UDPSocketOut != INVALID_SOCKET)
	{
		if (closesocket(UDPSocketIn) != 0)
		{
			cmd->PrintToCMD("[ERROR] error closing UDP Out Socket.");
		}
	}

	WSACleanup();
	exit(0);
}


void NetworkManager::ShutdownApplication()
{
	isThreadRunning = false;
	listenThread.join();
	Shutdown();
}


