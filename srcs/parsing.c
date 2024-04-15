#include "../includes/ft_malcolm.h"

void exit_error(char *message) {
    fprintf(stderr, "%s\n", message);
    free_data();
    exit(EXIT_FAILURE);
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


static _Bool is_valid_hex(char c) {
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

u_int8_t hextouint8(char *hex) {
    u_int8_t value = 0;
    for (int i = 0; i < 2; i++) {
        value *= 16;
        if (hex[i] >= '0' && hex[i] <= '9')
            value += hex[i] - '0';
        else if (hex[i] >= 'a' && hex[i] <= 'f')
            value += hex[i] - 'a' + 10;
        else if (hex[i] >= 'A' && hex[i] <= 'F')
            value += hex[i] - 'A' + 10;
    }
    return (value);
}

void parse_mac_address(char *mac) {
    int i = 0;
    if (ft_strlen(mac) != STR_MAC_LEN) {
        exit_error("ft_malcolm: Invalid MAC address format");
    }
    while (mac[i]) {
        if (i % 3 == 2 && mac[i] != ':')
            exit_error("ft_malcolm: Invalid MAC address format");
        if (i % 3 != 2 && !is_valid_hex(mac[i]))
            exit_error("ft_malcolm: Invalid MAC address format");
        i++;
    }
}

// int main(int ac, char **av) {
//     (void)ac;
//     parse_mac_address(av[1]);
//     for (int i = 0; i < 6; i++) {
//         printf("%s | value %d\n", av[1] + i * 3, hextouint8(av[1] + i * 3));
//     }
//     return (EXIT_SUCCESS);
// }