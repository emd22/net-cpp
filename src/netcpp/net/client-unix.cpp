#if defined(__linux__)

#include <net/client.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include <netdb.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
  
static bool end;

void client_error(const char *message) {
    printf("Client error: %s --- %s\n", message, strerror(errno));
    exit(1);
}

void cli_sig(int sig) {
    if (sig == SIGINT) {
        end = true;
    }
}

Client::Client(int port_, const std::string &ip) {
    signal(SIGINT, cli_sig);

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        client_error("Error opening socket");
    }
    //init socket struct
    memset((char *)&server, 0, sizeof(server));

    in_addr_t ipaddr = inet_addr(ip.c_str());

    if (ipaddr == INADDR_NONE) {
        client_error("Error with ipaddr");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = ipaddr;
    server.sin_port = htons(port_);

    server_ip = ip;
    port = port_;
    end = false;

    // int ret = inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);
    // if (ret <= 0) {
    //     client_error("Error with inet_pton\n");
    // }

    if (connect(client_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        client_error("Error connecting");
    }
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
    
    if (rsize == -1) {
        client_error("Error reading from server");
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