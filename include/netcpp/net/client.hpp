#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include <string>

#define DAT_SIZE 1024

#ifdef __linux__
#include <netinet/in.h>
#elif defined(_WIN32)
#include <winsock2.h>
#endif

class Client {
public:
    Client(int port, const std::string &ip);

    void listen(void *(*on_data_get)(const char *, Client *));
    char *read(char *dat);
    void send(const char *dat);
private:
    //#if defined(__linux__)
    int client_fd;
    int server_fd;
    sockaddr_in server, server_addr;
    int port;
    std::string server_ip;
    //#endif
};
#endif