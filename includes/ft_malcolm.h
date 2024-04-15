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

#define INTERFACE "eth0"
#define STR_MAC_LEN 17

/* ETHERNET HEADER */
typedef struct s_eth_hdr {
    u_int8_t mac_dest[ETHERNET_ADDR_LEN];
    u_int8_t mac_src[ETHERNET_ADDR_LEN];
    u_int16_t type;
} t_eth_hdr;

/* ARP HEADER */
typedef struct s_arp_hdr {
    u_int16_t hardware_type;
    u_int16_t proto_type;
    u_int8_t hardware_addr_len;
    u_int8_t proto_addr_len;
    u_int16_t operation; // request or reply
    u_int8_t mac_src[ETHERNET_ADDR_LEN];
    u_int32_t ip_src;
    u_int8_t mac_target[ETHERNET_ADDR_LEN];
    u_int32_t ip_target;
} t_arp_hdr;

/* ARP PACKET */
typedef struct s_arp_packet {
    t_eth_hdr eth_hdr;
    t_arp_hdr arp_hdr;
} t_arp_packet;


typedef struct s_data
{
	int					sockfd;
	struct addrinfo		*result;
	struct sockaddr_in	*sockaddr;
    u_int8_t            mac_src[ETHERNET_ADDR_LEN];
    u_int8_t            mac_target[ETHERNET_ADDR_LEN];
}	t_data;

extern t_data g_data;

/* Parsing */
void        exit_error(char *message);
u_int8_t    hextouint8(char *hex);
void        parse_mac_address(char *mac);

/* Display */
void        print_mac_address(u_int8_t *mac);
void        print_ip_address(u_int32_t ip);
void        print_packet(t_arp_hdr packet);

/* Main */
void free_data(void);

#endif