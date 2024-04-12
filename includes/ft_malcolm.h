#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/ip.h>
#include "../libft/libft.h" 

#define IP_SRC 1
#define MAC_SRC 2
#define IP_TARGET 3
#define MAC_TARGET 4

#define ARP_TYPE 0x0806

#define ETHERNET_10MB 0x0001
#define IPV4_TYPE 0x0800
#define ETHERNET_ADDR_LEN 6
#define IP_ADDR_LEN 4
#define REQUEST 0x0001
#define REPLY 0x0002

typedef struct s_arp_hdr {
    u_int16_t hardware_type;
    u_int16_t proto_type;
    u_int8_t hardware_addr_len;
    u_int8_t proto_addr_len;
    u_int16_t operation; // request or reply
    char mac_src[ETHERNET_ADDR_LEN];
    char ip_src[IP_ADDR_LEN];
    char mac_target[ETHERNET_ADDR_LEN];
    char ip_target[IP_ADDR_LEN];
} t_arp_hdr;


typedef struct s_data
{
	int					sockfd;
	struct addrinfo		*result;
	struct sockaddr_in	*sockaddr;
}	t_data;

extern t_data g_data;
