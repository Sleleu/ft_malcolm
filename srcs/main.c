#include "../includes/ft_malcolm.h"

void exit_error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv) {
    (void)argv;
    if (getuid() != 0)
        exit_error("ft_malcolm: Operation not permitted");
    if (argc != 5)
        exit_error("ft_malcolm: usage: <IP-src> <MAC-src> <IP-target> <MAC-target>");
    return (EXIT_SUCCESS);
}