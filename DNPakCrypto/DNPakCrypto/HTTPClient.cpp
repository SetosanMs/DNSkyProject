#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include "HTTPClient.h"
#include "../../DNSkyProject/se.h"
#include "../../DNSKyProject/Misc.h"
#pragma comment(lib,"ws2_32.lib")
HTTPClient gHTTP;


HTTPClient::HTTPClient()
{
}


HTTPClient::~HTTPClient()
{
}


void HTTPClient::Request(eRequest reqType, std::string URL)
{

}


void HTTPClient::Request(eRequest reqType, std::string URL, std::string data)
{

}


bool HTTPClient::SendCollectorData(eRequest reqType, std::string data, DWORD dataSize){
	
	//check ? ja check
	if (reqType != eRequest::POST)
	{
		return false;
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
#ifdef DEBUG
		std::cout << "WSAStartup failed.\n";
		system("pause");
#endif
		return FALSE;
	}
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct hostent *host;
	host = gethostbyname(enc("guard.ralukat.eu"));
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);


	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0){
#ifdef DEBUG
		std::cout << "Could not connect";
		system("pause");
#endif
		return FALSE;
	}
#ifdef DEBUG
	std::cout << "Connected.\n";
#endif
	std::string Header = enc("POST /metrix HTTP/1.1\r\nHost: guard.ralukat.eu\r\nUser-Agent: dragonnest.ro\r\nContent-Type: application/x-www-form-urlencoded\r\n");
	
	//a = username
	//b = ip
	//c = data
	std::string PostData;
	PostData.append(enc("a=") + AccountName);
	PostData.append(enc("&b=") + IPAddr);
	PostData.append(enc("&c=") + data);
	
	//calculam content-length pt ca php sucks.
	Header.append(enc("Content-Length: ") + std::to_string(PostData.length()));
	Header.append(enc("\r\n\r\n"));

	//il transferam in header
	Header.append(PostData);

	send(Socket, Header.c_str(), Header.size(), 0);
// doar de test.
#ifdef DEBUG
	char buffer[10000];
	int nDataLength;
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0){
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
			std::cout << buffer[i];
			i += 1;
		}
	}
#endif
	closesocket(Socket);
	WSACleanup();
//	system("pause");
	return TRUE;
}