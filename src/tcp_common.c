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

/* ——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 * level	|	option_name	|	data type	|	note		
 * SOL_SOCKET   |	SO_DEBUG	|	int		|	打开调试信息	
 * 			SO_REUSEADDR    |       int		|	使用本地地址	
 * 			SO_TYPE		|	int		|	获取socket类型	
 * 			SO_ERROR	|	int		|	获取并清除socket错误状态
 * 			SO_DONTROUTE	|	int		|	不查看路由表，直接将数据发送给
 * 									本地局域网内的主机。含义和send
 * 									系统调用的MSG_DONTROUTE标志类似
 * 			SO_RCVBUF	|	int		|	TCP接收缓冲区大小
 * 			SO_SNDBUF	|	int		|	TCP发送缓冲区大小
 * 			SO_KEEPALIVE	|	int		|	发送周期性保活报文以维持连接
 * 			SO_OOBINLIINE	|	int		|	接收到的带外数据将存留在普通数据的输入队列中。
 * 									此时我们不能使用带MSG_OOB标志的读操作来读取
 * 									带外数据
 * 			SO_LINGER	|	int		|	若有数据待发送，则延迟关闭
 * 			SO_RCVLOWAT	|	int		|	TCP接收缓存区域水位标记
 * 			SO_SNDLOWAT	|	int		|	TCP发送缓存区域水位标记
 * 			SO_RCVTIMEO	|	timeval		|	接收数据超时
 * 			SO_SNDTIMEO	|	timeval		|	发送数据超时
 * _____________________________________________________________________________________________________________________
 * IPPROTO_IP	|	IP_TOS		|	int		|	服务类型
 * 			IP_TTL		|	int		|	存活时间
 * —————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
 * IPPROTO_TCP	|	TCP_MAXSEG	|	int		|	TCP最大报文段大小
 * 			TCP_NODELAY	|	int		|	禁止Nagle算法
 * _____________________________________________________________________________________________________________________
 *
 */
int tcp_set_socketflag(int fd, int level, int option_name, void* option_value, socklen_t option_len)
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
