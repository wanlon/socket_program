#ifndef PRO_COMMON_EXT_H_
#define PRO_COMMON_EXT_H_

extern struct proto_socket_ops tcp_socket_ops;

const struct proto_socket_ops *const socket_ops[] =
{
    &tcp_socket_ops,
    NULL
};

#endif /* PRO_COMMON_EXT_H_ */
