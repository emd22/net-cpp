#if defined(__linux__)

#include <net/server.hpp>

#include <algorithm>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <errno.h>

bool end = false;

void server_error(const char *message) {
    printf("Client error: %s --- %s\n", message, strerror(errno));
    exit(1);
}

void serv_sig(int sig) {
    if (sig == SIGINT) {
        end = true;
    }
}

Server::Server(int port, const std::string &ip) {
    signal(SIGINT, serv_sig);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        server_error("Error opening socket");
    }

    memset((char *)&addr, 0, sizeof(addr));

    in_addr_t ipaddr = inet_addr(ip.c_str());

    if (ipaddr == INADDR_NONE) {
        server_error("Error with ipaddr");
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ipaddr;
    addr.sin_port = htons(port);

    fcntl(server_fd, F_SETFL, O_NONBLOCK);    
    int ret = bind(server_fd, (sockaddr *)&addr, sizeof(addr));

    if (ret < 0) {
        server_error("Error with binding. are you using the right network adapter?");
    }
}

void Server::listen(void *(*on_connect)(int, Server *)) {
    ::listen(server_fd, 5);
    sockaddr_in cli_addr;

    cli_len = sizeof(cli_addr);

    while (true) {
        new_sockfd = accept(server_fd, (sockaddr *)&cli_addr, (socklen_t *)&cli_len);

        while (errno == EWOULDBLOCK) {
            if (end)
                return;
        }

        if (new_sockfd < 0) {
            server_error("Error with accepting client");
        }
        client_fds.push_back(new_sockfd);
        on_connect(new_sockfd, this);

        Server::kick(new_sockfd, "skidaddle skidoodle, your weiner is now a noodle");
        // close(new_sockfd);
    }
}

char *Server::read(int fd, char *dat) {
    int rsize = 0;

    memset(dat, 0, 1024);

    rsize = recv(fd, dat, 1024, 0);
    if (rsize == -1) {
        server_error("Error reading from client");
        return NULL;
    }
    if (rsize == 0)
        return NULL;
    return dat;
}

void Server::send(int fd, const std::string &message) {
    int ret = 0;

    ret = ::send(fd, message.c_str(), message.size(), 0);
    if (ret < 0) {
        server_error("Error sending to client");
    }
    if (ret == 0) {
        int pos = 0;
        for (; pos < client_fds.size(); pos++) {
            if (client_fds[pos] == fd) {
                client_fds.erase(client_fds.begin()+pos);
                printf("Client %d left.\n", pos);
                break;
            }
        }
    }
}

void Server::broadcast(const std::string &message) {
    int len = client_fds.size();

    for (int i = 0; i < len; i++) {
        Server::send(client_fds[i], message);
    }
}

void Server::kick(int fd, const std::string &msg) {
    Server::send(fd, msg);
    close(fd);
}

std::vector<Data> Server::read_all() {
    std::vector<Data> dat_vec;
    int len = client_fds.size();

    char *dat = (char *)malloc(DAT_SIZE);

    for (int i = 0; i < len; i++) {
        if (Server::read(client_fds[i], dat) != NULL) {
            Data data;
            data.dat = dat;
            data.fd = client_fds[i];
            data.index = i;
            dat_vec.push_back(data);
        }
    }
    
    free(dat);
    return dat_vec;
}

#endif
