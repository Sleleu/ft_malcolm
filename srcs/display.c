#include "../includes/ft_malcolm.h"

void print_mac_address(u_int8_t *mac) {
    for (int i = 0; i < ETH_ALEN; i++) {
        printf(YELLOW "%02x", mac[i]);
        if (i < ETH_ALEN - 1)
            printf(":");
    }
    printf(WHITE);
}

void print_ip_address(u_int32_t ip) {
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip, str, INET_ADDRSTRLEN);
    printf(GREEN "%s\n" WHITE, str);
}

static const char* get_color_for_index(int i) {
    if (i <= 11 || (i >= 22 && i <= 27) || (i >= 32 && i <= 37))
        return YELLOW;
    else if (i > 32 || (i >= 28 && i <= 31))
        return GREEN;
    else if (i == 12 || i == 13)
        return PURPLE;
    else if (i == 16 || i == 17)
        return BLUE;
    else if (i == 18 || i == 19)
        return RED;
    else if (i == 20 || i == 21)
        return CYAN;
    else
        return WHITE;
}

static void print_hex_packet(t_arp_packet packet) {
    uint8_t *packet_ptr = (uint8_t *)&packet;
    printf("|\t\t\tHex dump of ARP packet:\t\t\t |\n| (0x0000)\t");
    for (int i = 0; i < (int)sizeof(t_arp_packet); i++) {
            printf("%s%02X "WHITE, get_color_for_index(i), packet_ptr[i]);
        if ((i + 1) % 16 == 0)
            printf(" |\n| (0x%04x)\t", i+1);
    }
    printf("\t\t\t |\n");
}

void print_packet(t_arp_packet packet) {
    printf(WHITE"==================== ARP PACKET ==================================\n");
    printf("\t---------- ETH HEADER ----------\n");
    printf("| Hardware target: ");
    print_mac_address(packet.h_target);
    printf(" | Hardware source: ");
    print_mac_address(packet.h_src);
    printf(" | Protocol: "PURPLE"ARP (0x%04x)"WHITE"\n", ntohs(packet.h_proto));
    printf("\t---------- ARP HEADER ----------\n");
    printf("| Hardware type: 0x%04X      | Protocol type: "BLUE"IPV4 (0x%04X)"WHITE"\n", ntohs(packet.hardware_type), ntohs(packet.proto_type));
    printf("| Hardware address len: "RED"0x%02X"WHITE" | Protocol address len: "RED"0x%02X"WHITE"\n", packet.hardware_addr_len, packet.proto_addr_len);
    if (ntohs(packet.operation) == REQUEST)
        printf("| Operation: REQUEST (0x%04X)\n", ntohs(packet.operation));
    else
        printf("| Operation: "CYAN"REPLY (0x%04X)"WHITE"\n", ntohs(packet.operation));
    printf("| Sender hardware address: ");
    print_mac_address(packet.mac_src);
    printf(" | Sender protocol address: ");
    print_ip_address(packet.ip_src);
    printf("| Target hardware address: ");
    print_mac_address(packet.mac_target);
    printf(" | Target protocol address: ");
    print_ip_address(packet.ip_target);
    printf(WHITE"==================================================================\n");
    print_hex_packet(packet);
    printf(WHITE"==================================================================\n");
    printf(END);
}
