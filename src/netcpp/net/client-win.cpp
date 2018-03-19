#ifdef __WIN32
#include <net/client.hpp>

#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

Client::Client(int port, const std::string &ip) {
	SOCKET client_fd = INVALID_SOCKET;
	addrinfo *result = NULL, *ptr = NULL, hints;

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create windows socket. This is hot steamy garbage, go download ubuntu please.\n");
		WSACleanup();
		exit(1);
	}

	struct sockaddr_in server;

	//I feel like i've seen this somewhere before...

	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(client_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Uh oh! it looks like winsock can't connect to the ip %s with the port %d.\n", ip, port);
	}
}

#endif