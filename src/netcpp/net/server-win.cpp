#ifdef _WIN32

#include <net/server.hpp>

#include <signal.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

static bool end = false;

void serv_sig(int sig) {
    if (sig == SIGINT)
        end = true;
}

Server::Server(int port, const std::string &ip) {
    signal(SIGINT, serv_sig);

    server_fd = INVALID_SOCKET;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create windows socket. Possibly not enough file descriptors?\n");
        WSACleanup();
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (connect(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        printf("Winsock cannot connect to the server\n");
        WSACleanup();
        exit(1);
    }
}

#endif