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
        printf("Exit with signal\n");
        free_data();
        exit(EXIT_SUCCESS);
    }
}

struct addrinfo *get_addrinfo(char *ip)
{
	struct addrinfo 	hints;
    struct addrinfo 	*result;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_ICMP;
	int status = getaddrinfo(ip, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "ft_malcolm: unknow host or invalid IP address: (%s).", ip);
        exit_error("");
    }
    return result;
}


t_arp_hdr create_arp_packet(char **argv) {
    t_arp_hdr packet;
    packet.hardware_type = htons(ETHERNET_10MB);
    packet.proto_type = htons(IPV4_TYPE);
    packet.hardware_addr_len = ETHERNET_ADDR_LEN;
    packet.proto_addr_len = IP_ADDR_LEN;
    packet.operation = htons(REPLY);

    if (!inet_pton(AF_INET, argv[IP_SRC], &packet.ip_src)
        || !inet_pton(AF_INET, argv[IP_TARGET], &packet.ip_target))
            exit_error("ft_malcolm: Invalid IP address format");

    for (int i = 0; i < ETHERNET_ADDR_LEN; i++) {
        packet.mac_src[i] = g_data.mac_src[i];
        packet.mac_target[i] = g_data.mac_target[i];
    }
    return (packet);
}

int     init_socket(void)
{
	int sockfd;

	sockfd = socket(AF_PACKET, SOCK_RAW, htons(ARP_TYPE));
	if (sockfd == -1)
		exit_error("ft_malcolm: socket error");
	return (sockfd);
}

void  check_interface(void) {
    struct ifreq ifr;
    ft_strlcpy(ifr.ifr_name, INTERFACE, IFNAMSIZ + 1);
    if (ioctl(g_data.sockfd, SIOCGIFINDEX, &ifr) == -1)
        exit_error("ft_malcolm: no such interface");
    printf("Found available interface: %s\n", ifr.ifr_name);
}

void	get_ipstr(char* ip_buffer, struct sockaddr_in* sockaddr) {
    if (sockaddr->sin_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)sockaddr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ip_buffer, INET_ADDRSTRLEN);
    }
    else
        exit_error("ft_malcolm: Invalid IP address");
}


void parse_ip_address(char *ip_src, char *ip_target) {
    g_data.result_src = get_addrinfo(ip_src);
    g_data.result_target = get_addrinfo(ip_target);
    g_data.sockaddr_src = (struct sockaddr_in *)g_data.result_src->ai_addr;
    g_data.sockaddr_target = (struct sockaddr_in *)g_data.result_target->ai_addr;
    g_data.ip_src = g_data.sockaddr_src->sin_addr.s_addr;
    g_data.ip_target = g_data.sockaddr_target->sin_addr.s_addr;
    
    get_ipstr(g_data.ip_src_str, g_data.sockaddr_src);
    get_ipstr(g_data.ip_target_str, g_data.sockaddr_target);

}

int main(int argc, char **argv) {
    if (getuid() != 0)
        exit_error("ft_malcolm: Operation not permitted");
    if (argc != 5)
        exit_error("ft_malcolm: usage: <IP-src> <MAC-src> <IP-target> <MAC-target>");

    parse_mac_address(argv[MAC_SRC]);
    parse_mac_address(argv[MAC_TARGET]);
    parse_ip_address(argv[IP_SRC], argv[IP_TARGET]);

    printf("g_data SRC : %s\n", g_data.ip_src_str);
    printf("g_data TARGET : %s\n", g_data.ip_target_str);
    printf("g_data SRC decimal %u\n", g_data.ip_src);
    printf("g_data TARGET decimal %u\n", g_data.ip_target);

    signal(SIGINT, signal_handler);
    g_data.sockfd = init_socket();
    check_interface();

    for (int i = 0; i < 6; i++) {
        g_data.mac_src[i] = hextouint8(argv[MAC_SRC] + i * 3);
        g_data.mac_target[i] = hextouint8(argv[MAC_TARGET] + i * 3);
    }

    t_arp_hdr packet = create_arp_packet(argv);
    print_packet(packet);
    free_data();
    return (EXIT_SUCCESS);
}