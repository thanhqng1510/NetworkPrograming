#define _AFXDLL 1


#include <iostream>
#include <afxsock.h>


#define PORT 1234
#define BUFF_LEN 1024
#define SERVER_IP "127.0.0.1"



int main() {
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		std::cout << "MFC initialization failed\n";
		return -1;
	}

	if (!AfxSocketInit()) {
		std::cout << "Socket library initialization failed\n";
		return -1;
	}

	CSocket client;

	client.Create();

	if (!client.Connect(_T(SERVER_IP), PORT)) {
		std::cout << "Can not connect to this ip: " << SERVER_IP << "\n";
		client.Close();
		return 0;
	}

	do {
		char msg[BUFF_LEN];

		client.Receive(msg, BUFF_LEN);

		std::cout << "Server: " << msg << "\n";

		int msg_size;
		std::cout << "Client: ";
		memset(msg, 0, BUFF_LEN);
		std::cin.getline(msg, BUFF_LEN);
		msg_size = strlen(msg);

		client.Send(msg, msg_size + 1);
	} while (true);

	client.Close();

	return 0;
}