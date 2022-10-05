//#include "NetworkManager.h"
//#include "iostream"
//#include  <WS2tcpip.h>
//#include "CommandInputHandler.h"
//
//using namespace std;
//
//NetworkManager* NetworkManager::instance = nullptr;
//
//NetworkManager::NetworkManager()
//{
//	
//}
//
//NetworkManager::~NetworkManager()
//{
//
//}
//
//void NetworkManager::Init(/*CommandInputHandler* cmdInstance*/)
//{
//	//cmd = cmdInstance;
//
//	//cmd->PrintToCMD("NetworkManager::Init() called.");
//
//	WSADATA lpWSAData;
//	int error = WSAStartup(MAKEWORD(2, 2), &lpWSAData);
//
//	if (error != 0)
//	{
//		//cmd->PrintToCMD("WSAData failed with error code: " + error);
//	}
//}
//
//void NetworkManager::Shutdown()
//{
//	cout << "NetworkManager::Shutdown() called." << endl;
//
//	int errorCode = WSAGetLastError();
//	if (errorCode != 0)
//	{
//		cout << "Forced shutdown due to WSAError#: " << errorCode << endl;
//	}
//
//	if (UDPSocketIn != INVALID_SOCKET)
//	{
//		if (closesocket(UDPSocketIn) != 0)
//		{
//			cout << "[ERROR] error closing UDP In Socket." << endl;
//		}
//	}
//
//	if (UDPSocketOut != INVALID_SOCKET)
//	{
//		if (closesocket(UDPSocketIn) != 0)
//		{
//			cout << "[ERROR] error closing UDP Out Socket." << endl;
//		}
//	}
//
//	WSACleanup();
//	exit(0);
//}
//
//void NetworkManager::CreateUDPSockets()
//{
//	u_long isBlocking = 1;
//
//	UDPSocketIn = socket(AF_INET, SOCK_DGRAM, 0);
//
//	ioctlsocket(UDPSocketIn, FIONBIO, &isBlocking);
//	
//	if (UDPSocketIn == INVALID_SOCKET)
//	{
//		cout << "Failed to create inbound socket" << endl;
//		Shutdown();
//	}
//
//	UDPSocketOut = socket(AF_INET, SOCK_DGRAM, 0);
//
//	if (UDPSocketOut == INVALID_SOCKET)
//	{
//		cout << "Failed to create outbound socket" << endl;
//		Shutdown();
//	}
//}
//
//void NetworkManager::BindUDP()
//{
//	// Using IPv4
//	inAddr.sin_family = AF_INET;
//
//	//Port 8889
//	inAddr.sin_port = htons(8889);
//
//	//From any available address (Computers can have multiple)
//	inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//	int bindError = bind(UDPSocketIn, reinterpret_cast<SOCKADDR*>(&inAddr), sizeof(inAddr));
//
//	if (bindError == SOCKET_ERROR)
//	{
//		cout << "[ERROR] binding failed." << endl;
//
//		Shutdown();
//	}
//}
//
//void NetworkManager::SetRemoteData(int port, string cxIP)
//{
//	outAddr.sin_family = AF_INET;
//
//	outAddr.sin_port = htons(port);
//
//	inet_pton(AF_INET, cxIP.c_str(), &outAddr.sin_addr);
//}
//
//void NetworkManager::SendData(const char* data)
//{
//	int totalBytes = sendto(UDPSocketOut, data, strlen(data) + 1, 0, 
//		reinterpret_cast<SOCKADDR*>(&outAddr), sizeof(outAddr));
//
//	if (totalBytes == SOCKET_ERROR)
//	{
//		Shutdown();
//	}
//
//	cout << " sent : " << totalBytes << " of data" << endl;
//}
//
//int NetworkManager::ReceiveData(char* ReceiveBuffer)
//{
//	int BytesReceived = 0;
//	int inAddrSize = sizeof(inAddr);
//
//	BytesReceived = recvfrom(UDPSocketIn, ReceiveBuffer, 65535, 0,
//		reinterpret_cast<SOCKADDR*>(&inAddr), &inAddrSize);
//
//	if (BytesReceived == WSAEWOULDBLOCK)
//	{
//		Shutdown();
//	}
//
//	return BytesReceived;
//}
//
//
