#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <ifaddrs.h>
#include "../libft/libft.h" 

#define IP_SRC 1
#define MAC_SRC 2
#define IP_TARGET 3
#define MAC_TARGET 4

#define ARP_TYPE 0x0806
#define ETHERNET_10MB 0x0001
#define IPV4_TYPE 0x0800
#define IP_ADDR_LEN 4
#define REQUEST 0x0001
#define REPLY 0x0002
#define INTERFACE "eth0"
#define STR_MAC_LEN 17


/* ARP PACKET */
typedef struct __attribute__((packed)) s_arp_packet {
    /* ETHERNET HEADER */
	u_int8_t	h_target[ETH_ALEN];
	u_int8_t    h_src[ETH_ALEN];
	u_int16_t   h_proto;

    /* ARP HEADER */
    u_int16_t   hardware_type;
    u_int16_t   proto_type;
    u_int8_t hardware_addr_len;
    u_int8_t proto_addr_len;
    u_int16_t operation;
    u_int8_t mac_src[ETH_ALEN];
    u_int32_t ip_src;
    u_int8_t mac_target[ETH_ALEN];
    u_int32_t ip_target;
} t_arp_packet;


typedef struct s_data
{
	int					sockfd;
	struct addrinfo		*result_src;
    struct addrinfo     *result_target;
	struct sockaddr_in	*sockaddr_src;
    struct sockaddr_in  *sockaddr_target;
    u_int8_t            mac_src[ETH_ALEN];
    u_int8_t            mac_target[ETH_ALEN];
    char                ip_src_str[INET_ADDRSTRLEN];
    char                ip_target_str[INET_ADDRSTRLEN];
    u_int32_t           ip_src;
    u_int32_t           ip_target;
}	t_data;

extern t_data g_data;

/* Parsing */
struct addrinfo *get_addrinfo(char *ip);
void        exit_error(char *message);
void	    get_ipstr(char* ip_buffer, struct sockaddr_in* sockaddr);
void        parse_ip_address(char *ip_src, char *ip_target);
u_int8_t    hextouint8(char *hex);
void        parse_mac_address(char *mac);

/* Display */
void        print_interface(struct ifaddrs *ifap);
void        print_mac_address(u_int8_t *mac);
void        print_ip_address(u_int32_t ip);
void        print_packet(t_arp_packet packet);

/* Main */
void free_data(void);

#endif