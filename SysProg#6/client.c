#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define on_error(...)                                                          \
  {                                                                            \
    fprintf(stderr, __VA_ARGS__);                                              \
    fflush(stderr);                                                            \
    exit(1);                                                                   \
  }

int main(int argc, char **argv) {
  int sockfd, n, err;
  char sendline[BUFFER_SIZE];
  char recvline[BUFFER_SIZE];

  if (argc < 3)
    on_error("Usage: %s [server's address] [port]\n", argv[0]);
  int port = atoi(argv[2]);

  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof servaddr);

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  inet_pton(AF_INET, argv[1], &(servaddr.sin_addr));

  err = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (err < 0)
    on_error("Could not bind socket\n");

  while (1) {
    bzero(sendline, BUFFER_SIZE);
    bzero(recvline, BUFFER_SIZE);
    fgets(sendline, BUFFER_SIZE, stdin); /*stdin = 0 , for standard input */

    send(sockfd, sendline, strlen(sendline) + 1, 0);
    recv(sockfd, recvline, strlen(sendline) + 1, 0);
    printf("Receiver from server: %s", recvline);
  }
}
