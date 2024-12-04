#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

int create_tcp_server_socket()
{
  // 接続を待ち受けるためのソケットを生成
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("Socket Error");
    exit(EXIT_FAILURE);
  };

  // サーバーのアドレス情報をソケットに割り当てる
  struct sockaddr_in server_addr;
  char *server_ip = "192.168.100.10";
  unsigned short server_port = 8000;
  int one = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
  memset(&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(server_port);
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    perror("Bind Error");
    exit(EXIT_FAILURE);
  };

  // 接続を待ち受ける状態にする
  if (listen(sockfd, 1000) != 0)
  {
    perror("Listen Error");
    exit(EXIT_FAILURE);
  };

  return sockfd;
}

void accept_new_connection_request(int sockfd, int epollfd)
{
  struct sockaddr_in new_addr;
  socklen_t addr_size = sizeof(new_addr);
  while (1)
  {
    /* 新しい接続を受け入れる */
    int connfd = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    if (connfd < 0)
    {
      if ((errno = EAGAIN) || (errno == EWOULDBLOCK))
      {
        /* 全ての接続要求を処理済み */
        break;
      }
      else
      {
        perror("Accept Error");
        break;
      }
    }

    /* 新しく生成したソケットをノンブロッキングにする */
    fcntl(connfd, F_SETFL, O_NONBLOCK);

    /* 新しいソケットを監視 */
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = connfd;

    /* epoll に新しく生成したソケットを監視させる */
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1)
    {
      perror("Epoll Ctl Error");
      break;
    }
  }
}

void recv_and_send_message(int fd)
{
  /*メッセージを受け取る */
  char buffer[1024];
  bzero(buffer, 1024);
  int ret_data = recv(fd, buffer, sizeof(buffer), 0);

  if (ret_data < 0)
  {
    perror("Recv Error");
  }
  else if (ret_data > 0)
  {
    printf("[Recv] Client(%d): %s\n", fd, buffer);

    /* クライアントにメッセージを送り返す */
    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS MULTI-CLIENT SERVER.");
    if (send(fd, buffer, strlen(buffer), 0) < 0)
    {
      perror("Send Error");
      exit(EXIT_FAILURE);
    }
  }
}
