#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
  char *server_ip = "192.168.100.10"; /* サーバーのIPアドレス */
  unsigned short server_port = 8000;  /* サーバーのポート番号 */

  int sockfd;                     /* 接続を待ち受けるためのソケットのファイルディスクリプタ */
  char buffer[1024];              /* データ送受信のためのソケットのバッファサイズ */
  struct sockaddr_in server_addr; /* サーバーのアドレス情報 */

  /* ソケットを生成 */
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Socket Error");
    exit(EXIT_FAILURE);
  }
  printf("[Socket] Socket successfully created. fd: %d\n", sockfd);

  /* サーバーに接続する */
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(server_port);
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    perror("Connect Error");
    exit(EXIT_FAILURE);
  }
  printf("[Connect] Connected to the server.\n");

  /* サーバーにメッセージを送信する */
  bzero(buffer, 1024);
  strcpy(buffer, "HELLO, THIS IS CLIENT.");
  if (send(sockfd, buffer, strlen(buffer), 0) < 0)
  {
    perror("Send Error");
    exit(EXIT_FAILURE);
  }
  printf("[Send] Message successfly sent.\n");

  /* サーバーからメッセージを受け取る */
  bzero(buffer, 1024);
  if (recv(sockfd, buffer, sizeof(buffer), 0) < 0)
  {
    perror("Recv Error");
    exit(EXIT_FAILURE);
  }
  printf("[Recv] Server: %s\n", buffer);

  /* 切断 */
  close(sockfd);
  printf("[Close] Server disconnected.\n\n");
}