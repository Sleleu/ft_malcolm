#include "../includes/ft_malcolm.h"

void exit_error(char *message) {
    fprintf(stderr, "%s\n", message);
    free_data();
    exit(EXIT_FAILURE);
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