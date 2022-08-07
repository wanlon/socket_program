#ifndef PRO_COMMON_H_
#define PRO_COMMON_H_

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define NAEM_LEN   64
#define IP_LEN   20

struct socket_data {
    int fd;
    int cli_fd;
    int port;
    unsigned char server_ip[IP_LEN];

    struct sockaddr_in servaddr, cli;

    unsigned char backlog;
};

struct proto_socket_ops {
    const char *name;

    int (*create)(struct socket_data *socket);
    int (*set_socket_opt)(int fd, int level, int option_name, void* option_value, socklen_t option_len);
    int (*bind)(struct socket_data *socket);
    int (*listen)(struct socket_data *socket);
    int (*accept)(struct socket_data *socket);
    int (*close)(int fd);
    int (*connect)(struct socket_data *socket);
};

#endif /* PRO_COMMON_H_ */


