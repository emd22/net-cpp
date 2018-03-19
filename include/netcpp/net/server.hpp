#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

#if defined(__linux__)
#include <netinet/in.h>
#elif defined(_WIN32)
#include <winsock2.h>
#endif

#define DAT_SIZE 1024

struct Data {
    int fd;
    int index;
    std::string dat;
};

class Server {
public:
    Server(int port, const std::string &ip);

    void listen(void *(*on_connect)(int, Server *));
    char *read(int fd, char *dat);
    void send(int fd, const std::string &message);
    void broadcast(const std::string &message);
    void kick(int index, const std::string &msg);
    std::vector<Data> read_all();
private:
    int server_fd, new_sockfd, cli_len;
    sockaddr_in addr;
    std::vector<int> client_fds;
};

#endif