#include "../includes/ft_malcolm.h"

void print_interface(struct ifaddrs *ifap) {
    char host[NI_MAXHOST];

    printf("==== Found available interfaces ====\n");
    while (ifap->ifa_next) {
        int family = ifap->ifa_addr->sa_family;
        printf("%-8s %s (%d)", ifap->ifa_name,
                      (family == AF_PACKET) ? "AF_PACKET" :
                      (family == AF_INET) ? "AF_INET" :
                      (family == AF_INET6) ? "AF_INET6" : "???",
                      family);
        ifap = ifap->ifa_next;
        if (family == AF_INET || family == AF_INET6) {
            int s = getnameinfo(ifap->ifa_addr, family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
                printf("\t\tgetnameinfo() failed");
            else
                printf("\t\taddress: <%s>", host);
            } else if (family == AF_PACKET && ifap->ifa_data != NULL) {
                   struct rtnl_link_stats *stats = ifap->ifa_data;
                   printf("\t\ttx_packets = %10u; rx_packets = %10u"
                          "tx_bytes   = %10u; rx_bytes   = %10u",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
               }
        printf("\n");
    }
    printf("===================================\n");
}

void print_mac_address(u_int8_t *mac) {
    for (int i = 0; i < ETHERNET_ADDR_LEN; i++) {
        printf("%02x", mac[i]);
        if (i < ETHERNET_ADDR_LEN - 1)
            printf(":");
    }
    printf("\n");
}

void print_ip_address(u_int32_t ip) {
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip, str, INET_ADDRSTRLEN);
    printf("%s\n", str);
}

void print_packet(t_arp_hdr packet) {
    printf("----- ARP HEADER -----\n");
    printf("TOTAL size: 28 bytes\n");
    printf("hardware type: %d\n", ntohs(packet.hardware_type));
    printf("protocol type: %d (IPV4)\n", ntohs(packet.proto_type));
    printf("hardware address len: %d\n", packet.hardware_addr_len);
    printf("protocole address len: %d\n", packet.proto_addr_len);
    printf("operation: %d\n", ntohs(packet.operation));
    printf("sender hardware address: ");
    print_mac_address(packet.mac_src);
    printf("sender protocol address: ");
    print_ip_address(packet.ip_src);
    printf("target hardware address: ");
    print_mac_address(packet.mac_target);
    printf("target protocol address: ");
    print_ip_address(packet.ip_target);
}
