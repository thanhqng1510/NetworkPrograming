#define _AFXDLL 1


#include <iostream>
#include <afxsock.h>


#define PORT 1234
#define BUFF_LEN 1024



int main() {
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		std::cout << "MFC initialization failed\n";
		return -1;
	}

	if (!AfxSocketInit()) {
		std::cout << "Socket library initialization failed\n";
		return -1;
	}

	CSocket server;

	if (!server.Create(PORT, SOCK_STREAM, NULL)) {
		std::cout << "Socket creation failed\n";
		std::cout << "Error code: " << server.GetLastError() << "\n";
		return -2;
	}

	if (!server.Listen(5)) {
		std::cout << "Can not listen on port " << PORT << "\n";
		server.Close();
		return 0;
	}

	std::cout << "Waiting for connection...\n";

	CSocket client;

	if (server.Accept(client)) {
		std::cout << "A client have connected\n";

		do {
			char msg[BUFF_LEN];

			int msg_size;
			std::cout << "Server: ";
			std::cin.getline(msg, BUFF_LEN);
			msg_size = strlen(msg);

			client.Send(msg, msg_size + 1);

			memset(msg, 0, BUFF_LEN);
			client.Receive(msg, BUFF_LEN);

			std::cout << "Client: " << msg << "\n";
		} while (true);

		client.Close();
		server.Close();
	}

	return 0;
}