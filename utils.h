#ifndef UTILS_H
#define UTILS_H

int create_tcp_server_socket();
void accept_new_connection_request(int sockfd, int epollfd);
void recv_and_send_message(int fd);
#endif
