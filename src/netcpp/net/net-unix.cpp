#if defined(__linux__)

#include <net/net.hpp>

#include <string>
#include <string.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>

char *Net::get_default_device() {
    FILE *f;
    char line[100], *p, *c;
    
    f = fopen("/proc/net/route", "r");
    
    while (fgets(line, 100, f)) {
        p = strtok(line, " \t");
        c = strtok(NULL, " \t");
        
        if (p != NULL && c != NULL) {
            if (strcmp(c , "00000000") == 0) {
                return p;
            }
        }
    }
}

std::string Net::get_ip(addr_t *addr) {
    in_addr ip_addr = addr->sin_addr;
    std::string ip;
    char cstr[32];

    inet_ntop(AF_INET, &ip_addr, cstr, IP_STR_LEN);

    ip.append(cstr);
    return ip;
}

std::string Net::get_host_ip(const char *adapter) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, adapter, IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    std::string ip;
    ip.append(inet_ntoa(((sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    return ip;
}

#endif