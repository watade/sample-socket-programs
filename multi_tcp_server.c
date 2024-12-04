#include "multi_tcp_server.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

int main(void)
{
  /* 接続を待ち受けるためのソケットを生成 */
  int sockfd = create_tcp_server_socket();

  /* ノンブロッキングなソケットにする */
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  /* MAX_EVENTS の数まで event を監視 */
  struct epoll_event ev, events[MAX_EVENTS];

  /* epoll のインスタンスを生成 */
  int epollfd = epoll_create1(0);
  if (epollfd < 0)
  {
    perror("Epoll Create Error");
    exit(EXIT_FAILURE);
  }

  /* Edge Triggered モードで読み込みイベントを監視 */
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = sockfd;

  /* epoll に sockfd ソケットを監視させる */
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1)
  {
    perror("Epoll Ctl Error");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    /* イベントがあったソケットの数 */
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds < 0)
    {
      perror("Epoll Wait Error");
      exit(EXIT_FAILURE);
    }

    /* イベントがあったソケットでループ */
    for (int i = 0; i < nfds; i++)
    {
      int fd = events[i].data.fd;

      if (fd == sockfd)
      {
        /* 新しい接続要求がある場合ソケットを生成する */
        accept_new_connection_request(fd, epollfd);
      }
      else if ((events[i].events & EPOLLERR) ||
               (events[i].events & EPOLLHUP) ||
               (!(events[i].events & EPOLLIN)))
      {
        /* 接続を切断する */
        close(fd);
      }
      else
      {
        /* 受信したデータを処理する */
        recv_and_send_message(fd);
      }
    }
  }
}