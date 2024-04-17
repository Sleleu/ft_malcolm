#include "../includes/ft_malcolm.h"

void print_mac_address(u_int8_t *mac) {
    for (int i = 0; i < ETH_ALEN; i++) {
        printf("%02x", mac[i]);
        if (i < ETH_ALEN - 1)
            printf(":");
    }
    printf("\n");
}

void print_ip_address(u_int32_t ip) {
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip, str, INET_ADDRSTRLEN);
    printf("%s\n", str);
}

void print_packet(t_arp_packet packet) {
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
