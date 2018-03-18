#ifndef NET_HPP
#define NET_HPP

#define FAIL 1
#define SUCCESS 0

#define CLIENT true
#define SERVER false

#define IP_STR_LEN 32

#include <net/server.hpp>
#include <net/client.hpp>

#include <vector>
#include <string>

#ifdef UNIX
#include <netinet/in.h>
typedef sockaddr_in addr_t;
#elif defined(_WIN32)
#include <winsock2.h>
typedef SOCKADDR_IN addr_t;
#endif

#define DAT_SIZE 1024

class Net {
public:
	Net();
    char *get_default_device();
    std::string get_ip(addr_t *addr);
    std::string get_host_ip(const char *adapter);
    ~Net();
private:
    #ifdef _WIN32
    WSADATA wsa_dat;
    #endif
};

#endif