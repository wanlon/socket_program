#include <stdio.h>
#include "proto_common.h"
#include "proto_common_ext.h"

int main()
{
    struct proto_socket_ops *ops;
    struct socket_data tcp_socket_data;

    int i;
    for (i=0; socket_ops[i]; i++) {
        if(strcmp("tcp", socket_ops[i]->name) == 0) {
            ops=socket_ops[i];
	    break;
        }
    }

    if(ops == NULL) {
	return -1;
    }

    if ((ops->create(&tcp_socket_data)) < 0) {
        return -1;
    }

    tcp_socket_data.port = 9000;

    if ((ops->bind(&tcp_socket_data)) < 0) {
        goto error;
    }

    if (ops->listen(&tcp_socket_data) < 0) {
        goto error;
    }

    /*
     * FD_CLR:清除某一个被监视的文件描述符
     * FD_ISSET：返回int值，测试某一个文件描述符是否是集合中的一员
     * FD_SET:添加一个文件描述符，将set中的某一位设置成1
     * FD_ZERO:清空集合中的文件描述符，将每一位都设置为0
     */

    fd_set rfds;
    fd_set rfds_tmp;
    struct timeval tv;
    int maxfd = tcp_socket_data.fd;

    FD_ZERO(&rfds);
    FD_SET(tcp_socket_data.fd, &rfds);
    FD_ZERO(&rfds_tmp);


    while (1) {
	char buf[256] = {0};
	int confd;


        tv.tv_sec=2;
        tv.tv_usec=0;

	rfds_tmp = rfds;
	//FD_ZERO(&rfds);
        //FD_SET(tcp_socket_data.fd, &rfds);
        //select每次返回后，会将没有就绪的文件描述符擦出，因此需要重新重置这个集合，即用rfds保存，tmp临使用
        int ret = select(maxfd+1, &rfds_tmp, NULL, NULL, &tv);
	if (ret < 0) {
            printf("select error\n");
        } else if (!ret) {
            printf("no data recive!!!\n");
        } else {
            for (int i=0; i<maxfd+1; i++) {
                if (FD_ISSET(i, &rfds_tmp)) {
                    if (i == tcp_socket_data.fd) {
                        if ((confd = ops->accept(&tcp_socket_data)) < 0) {
                            printf("accept error\n");
			    break;
			}
                        FD_SET(confd, &rfds);
			maxfd = (maxfd < confd ? confd : maxfd);
                    } else {
                        confd = i;
                        int ret1 = recv(confd, buf, sizeof(buf), 0);
			if (ret1 < 0) {
                            printf("recv error\n");
                            break;
                        } else if (!ret1) {
                            printf("client[%d] disconnect\n", confd);
                            FD_CLR(confd, &rfds);
			    ops->close(confd);
			    continue;
                        } else {
                            printf("client send data is: %s\n");
                        }
                    }
                }
	    }
        }



    }

error:
    ops->close(tcp_socket_data.fd);

    return -1;
}
