#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define IP_LEN   20
#define SA struct sockaddr

struct tcp_socket_data {
    int fd;
    int cli_fd;
    int port;
    char server_ip[IP_LEN];

    struct sockaddr_in servaddr, cli;

    unsigned char backlog;
}

int tcp_create_socket(struct tcp_socket_data *socket)
{
    int fd = socket->fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        prt_log("creat tcp sock error!!!");
        return -1;
    }

    return 0;
}

int tcp_set_socketflag()
{
    return 0;
}

int tcp_bind_socket(struct tcp_socket_data *socket)
{
    int fd = socket->fd;
    int port = socket->port;
    struct sockaddr_in *servaddr = socket->servaddr;

    bzero(servaddr, sizeof(sockaddr_in));

    servaddr->sin_family = AF_INET;
    (servaddr->sin_addr).s_addr = htonl(INADDR_ANY);
    servaddr->sin_port = htons(port);

    if ((bind(fd, (SA*)servaddr, sizeof(struct sockaddr_in))) != 0) {
        prt_log("tcp bind socket fail");
        return -1;
    }

    return 0;
}

int tcp_listen_socket(struct tcp_socket_data *socket)
{
    int fd = socket->fd;
    unsigned char backlog = socket->backlog;

    /*
     * the value of backlog should be less than 30
     */
    if ((listen(sockfd, backlog)) != 0) {
        prt_log("tcp listen socket fail");
        return -1;
    }

    return 0;
}

int tcp_accept_socket(struct tcp_socket_data *socket)
{
    int fd = socket->fd;
    struct sockaddr *cliaddr = socket->cli;
    int connect_fd = -1;

    connect_fd = accept(fd, (SA *)cliaddr, sizeof(SA));

    return connect_fd;
}

int tcp_close_socket(int fd)
{
    close(fd);

    return 0;
}

int tcp_connect_socket(struct tcp_socket_data *socket);
{
    struct sockaddr_in *cliaddr = socket->cli;
    const char* server_ip = socket->server_ip;
    int port = socket->port;

    bzero(cliaddr,sizeof(struct sockaddr_in));
    cliaddr->sin_family = AF_INET;

    (cliaddr->sin_addr).s_addr = inet_addr(server_ip);
    (cliaddr->sin_addr).sin_port = htons(port);

    int ret = connect(clifd, (SA *)cliaddr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        prt_log("cli connect server fail");
        return -1;
    }

    return 0;
}

static 
