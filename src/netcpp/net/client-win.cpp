#ifdef __WIN32
#include <net/client.hpp>

#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

Client::Client(int port_, const std::string &ip) {
	client_fd = INVALID_SOCKET;
	addrinfo *result = NULL, *ptr = NULL, hints;

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create windows socket. This is hot steamy garbage, go download ubuntu please.\n");
		WSACleanup();
		exit(1);
	}

	//I feel like i've seen this somewhere before...

    memset(&server, 0, sizeof(server));
	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (connect(client_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Uh oh! it looks like winsock can't connect to the ip %s with the port %d.\n", ip, port);
	}

    server_ip = ip;
    port = port_;
}

void Client::listen(void *(*on_data_get)(const char *, Client *)) {
    char *buf = (char *)malloc(DAT_SIZE);

    while (true) {
        if (Client::read(buf) != NULL) {
            on_data_get(buf, this);
        }
        if (end)
            break;
    }
    free(buf);
}

char *Client::read(char *dat) {
    int rsize = 0;
    
    memset(dat, 0, 1024);

    rsize = recv(client_fd, dat, 1024, 0);
    if (rsize < 0) {
        printf("Error reading from server\n");
        return NULL;
    }
    if (rsize == 0)
        return NULL;
    return dat;
}

void Client::send(const char *dat) {
    int ret = 0;

    // ret = connect(client_fd, (sockaddr *)&addr, sizeof(addr));
    // if (ret < 0) {
    //     client_error("Error with client connecting");
    // }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    ret = inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);
    if (ret <= 0) {
        client_error("Error with inet_pton\n");
    }

    if (::send(client_fd, dat, strlen(dat), 0) == -1) {
        client_error("Error sending from client");
    }
}

#endif