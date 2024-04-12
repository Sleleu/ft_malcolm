#include "../includes/ft_malcolm.h"

t_data g_data;

void exit_error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("Exit with signal\n");
        exit(EXIT_SUCCESS);
    }
}

void init_sockaddr(char *ip_target)
{
	struct addrinfo 	hints;
	struct addrinfo		*result;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_ICMP;
	int status = getaddrinfo(ip_target, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "ft_malcolm: unknow host or invalid IP address: (%s).\n", ip_target);
        exit(EXIT_FAILURE);
    }
    g_data.result = result;	
    g_data.sockaddr = (struct sockaddr_in *)result->ai_addr;
}


void print_debug(void) {
    printf("g_data.sockaddr->sin_addr.s_addr: %d\n", g_data.sockaddr->sin_addr.s_addr);

    // ARP PACKET
}

t_arp_hdr create_arp_packet(char **argv) {
    t_arp_hdr packet;
    packet.hardware_type = ETHERNET_10MB;
    packet.proto_type = IPV4_TYPE;
    packet.hardware_addr_len = ETHERNET_ADDR_LEN;
    packet.proto_addr_len = IP_ADDR_LEN;
    packet.operation = REPLY;
    ft_memcpy(packet.mac_src, argv[MAC_SRC], MAC_SRC);
    ft_memcpy(packet.ip_src, argv[IP_SRC], IP_SRC);
    ft_memcpy(packet.mac_target, argv[MAC_TARGET], MAC_TARGET);
    ft_memcpy(packet.ip_target, argv[IP_TARGET], IP_TARGET);
    return (packet);
}

void print_packet(t_arp_hdr packet) {
    printf("----- ARP HEADER -----\n");
    printf("TOTAL size: 28 bytes\n");
    printf("hardware type: %d\n", packet.hardware_type);
    printf("protocol type: %d (IPV4)\n", packet.proto_type);
    printf("hardware address len: %d\n", packet.hardware_addr_len);
    printf("protocole address len: %d\n", packet.proto_addr_len);
    printf("operation: %d\n", packet.operation);
    printf("sender hardware address: %s\n", packet.mac_src);
    printf("sender protocol address: %s\n", packet.ip_src);
    printf("target hardware address: %s\n", packet.mac_target);
    printf("target protocol address: %s\n", packet.ip_target);
}

int main(int argc, char **argv) {
    if (getuid() != 0)
        exit_error("ft_malcolm: Operation not permitted");
    if (argc != 5)
        exit_error("ft_malcolm: usage: <IP-src> <MAC-src> <IP-target> <MAC-target>");
    signal(SIGINT, signal_handler);

    init_sockaddr(argv[IP_TARGET]);
    t_arp_hdr packet = create_arp_packet(argv);
    print_packet(packet);
    //print_debug();
    return (EXIT_SUCCESS);
}