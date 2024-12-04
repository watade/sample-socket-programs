#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(void)
{
  char *server_ip = "192.168.100.10"; /* サーバーのIPアドレス */
  unsigned short server_port = 8000;  /* サーバーのポート番号 */

  int sockfd;                     /* 接続を待ち受けるためのソケットのファイルディスクリプタ */
  int connfd;                     /* クライアントと通信するためのソケットのファイルディスクリプタ */
  char buffer[1024];              /* データ送受信のためのソケットのバッファサイズ */
  struct sockaddr_in server_addr; /* サーバーのアドレス情報 */
  struct sockaddr_in client_addr; /* クライアントのアドレス情報 */
  socklen_t addr_size;            /* クライアントのアドレス情報の長さ*/
  char client_ip[32];             /* クライアントのIPアドレス */

  /* 接続を待ち受けるためのソケットを生成 */
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket Error");
    exit(EXIT_FAILURE);
  }
  printf("[Socket] Socket successfully created. fd: %d\n", sockfd);

  /* サーバーのアドレス情報をソケットに割り当てる */
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
  }
  printf("[Bind] Socket successfully bound.\n");

  /* 接続を待ち受ける状態にする */
  if (listen(sockfd, 5) != 0)
  {
    perror("Listen Error");
    exit(EXIT_FAILURE);
  }
  printf("[Listen] Listening...\n");

  /* 接続があれば受け入れ、新たなソケットを生成する */
  addr_size = sizeof(client_addr);
  if ((connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size)) < 0)
  {
    perror("Accept Error");
    exit(EXIT_FAILURE);
  }
  memset(client_ip, '\0', sizeof(client_ip));
  inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
  printf("[Accept] Client connected. fd: %d, Client IP: %s, Client Port: %u\n", connfd, client_ip, client_addr.sin_port);

  /* 生成されたソケットを使いメッセージを受け取る */
  bzero(buffer, 1024);
  if (recv(connfd, buffer, sizeof(buffer), 0) < 0)
  {
    perror("Recv Error");
    exit(EXIT_FAILURE);
  }
  printf("[Recv] Client: %s\n", buffer);

  /* クライアントにメッセージを送り返す */
  bzero(buffer, 1024);
  strcpy(buffer, "HELLO, THIS IS SERVER.");
  if (send(connfd, buffer, strlen(buffer), 0) < 0)
  {
    perror("Send Error");
    exit(EXIT_FAILURE);
  }
  printf("[Send] Message successfly sent.\n");

  /* 切断 */
  close(connfd);
  printf("[Close] Client disconnected.\n");

  close(sockfd);
  printf("[Close] Server ended.\n");
}