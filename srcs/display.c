#include "../includes/ft_malcolm.h"

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

void	get_ipstr(char* ip_buffer) {
    if (g_data.sockaddr->sin_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)g_data.sockaddr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip_buffer, INET_ADDRSTRLEN);
    }
    else
        exit_error("ft_malcolm: Invalid IP address");
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
    printf("sender hardware address: ");
    print_mac_address(packet.mac_target);
    printf("\n");
    printf("target protocol address: ");
    print_ip_address(packet.ip_target);
}
