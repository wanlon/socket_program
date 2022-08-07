#include <stdio.h>
#include "proto_common.h"

#define SA struct sockaddr_in

static int tcp_create_socket(struct socket_data *socket_d)
{
    int fd = socket_d->fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        prt_log("creat tcp sock error!!!");
        return -1;
    }

    socket_d->fd = fd;

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
static int tcp_set_socketflag(int fd, int level, int option_name, void* option_value, socklen_t option_len)
{

    return 0;
}

static int tcp_bind_socket(struct socket_data *socket_d)
{
    int fd = socket_d->fd;
    int port = socket_d->port;
    struct sockaddr_in *servaddr = &socket_d->servaddr;

    bzero(servaddr, sizeof(SA));

    servaddr->sin_family = AF_INET;
    (servaddr->sin_addr).s_addr = htonl(INADDR_ANY);
    servaddr->sin_port = htons(port);
    if ((bind(fd, servaddr, sizeof(SA))) < 0) {
        prt_log("tcp bind socket fail");
        return -1;
    }

    return 0;
}

static int tcp_listen_socket(struct socket_data *socket_d)
{
    int fd = socket_d->fd;
    unsigned char backlog = socket_d->backlog;

    /*
     * the value of backlog should be less than 30
     */
    if ((listen(fd, backlog)) != 0) {
        prt_log("tcp listen socket fail");
        return -1;
    }

    return 0;
}

static int tcp_accept_socket(struct socket_data *socket_d)
{
    int fd = socket_d->fd;
    struct sockaddr *cliaddr = &socket_d->cli;
    int connect_fd = -1;
    int len = sizeof(SA);

    connect_fd = accept(fd, (struct sockaddr*)cliaddr, &len);

    return connect_fd;
}

static int tcp_close_socket(int fd)
{
    close(fd);

    return 0;
}

static int tcp_connect_socket(struct socket_data *socket_d)
{
    struct sockaddr_in *cliaddr = &socket_d->cli;
    const char* server_ip = socket_d->server_ip;
    int port = socket_d->port;

    bzero(cliaddr,sizeof(struct sockaddr_in));
    cliaddr->sin_family = AF_INET;

    (cliaddr->sin_addr).s_addr = inet_addr(server_ip);
    cliaddr->sin_port = htons(port);
    int ret = connect(socket_d->cli_fd, (SA *)cliaddr, sizeof(SA));
    if (ret < 0) {
        prt_log("cli connect server fail");
        return -1;
    }

    return 0;
}

const struct proto_socket_ops tcp_socket_ops =
{
	.name   = "tcp",
	.create = tcp_create_socket,
	.set_socket_opt = tcp_set_socketflag,
	.bind = tcp_bind_socket,
	.listen = tcp_listen_socket,
	.accept = tcp_accept_socket,
	.close = tcp_close_socket,
	.connect = tcp_connect_socket
};
