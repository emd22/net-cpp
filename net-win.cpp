#ifdef _WIN32
#include <net/net.hpp>
#include <stdio.h>
#include <stdlib.h>

Net::Net() {   
    WSAStartup(MAKEWORD(2, 0), &wsa_dat);
}

char *Net::get_default_device() {
    char dev[64];
    gethostname(dev, 64);
    return dev;
}

std::string Net::get_ip(addr_t *addr) {
    return nullptr;
}

std::string Net::get_host_ip(const char *adapter) {
    hostent *hp = gethostbyname(adapter);

    if (hp == NULL) {
        printf("gethostbyname() failed, are you using the right ip? [adapter: %s]\n", adapter);
        exit(1);
    }

    char ip[48];

    in_addr *ia = ((in_addr *)(hp->h_addr));

    //why windows?

    sprintf(ip, "%c.%c.%c.%c", ia->S_un.S_un_b.s_b1, ia->S_un.S_un_b.s_b2, ia->S_un.S_un_b.s_b3, ia->S_un.S_un_b.s_b4);
    printf("IP: %s\n", ip);
    
    std::string fin;
    fin += ip;

    return fin;
}

Net::~Net() {
    WSACleanup();
}


#endif