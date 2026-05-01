#include <netinet/in.h>
#include <pthread.h>
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

struct req {
  int fd;
  socklen_t len;
  struct sockaddr_in addr;
};

void *handle_client_connection(void *arg) {

  // shoot myself in the foot diary
  // do no write this because the value at the address the pointer r points to
  // struct req *r = (struct req *)arg;
  // will be overwritten when the next call to accept() completes
  // as the r variable's lifetime lasts only one iteration of the loop
  // deference the pointer to use the value instead of the address
  //
  // Another way to solve this is to allocate argument passed to pthread_create
  // on the Heap
  struct req r = *(struct req *)arg;
  char buf[100];
  while (1) {
    bzero(buf, 100);
    int read = recv(r.fd, buf, 100, 0);

    if (read < 0)
      on_error("Client read failed\n");

    printf("Receive %d bytes from client_%d:%d: %s", read, r.fd,
           ntohs(r.addr.sin_port), buf);
    int err = send(r.fd, buf, strlen(buf), 0);
    if (err < 0)
      on_error("Client write failed\n");
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    on_error("Usage: %s [port]\n", argv[0]);

  int port = atoi(argv[1]);

  int server_fd, err;
  struct sockaddr_in server;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
    on_error("Could not create socket\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

  err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
  if (err < 0)
    on_error("Could not bind socket\n");

  err = listen(server_fd, 128);
  if (err < 0)
    on_error("Could not listen on socket\n");

  printf("Server is listening on %d\n", port);

  int no_threads = 0;
  pthread_t threads[3];
  while (no_threads < 3) {
    printf("Listening...\n");

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
    // int client_fd = accept(server_fd, NULL, NULL);
    struct req r;
    r.addr = client;
    r.len = client_len;
    r.fd = client_fd;
    puts("Connection accepted");
    if (pthread_create(&threads[no_threads], NULL, handle_client_connection,
                       &r) < 0) {
      perror("Could not create thread");
      return 1;
    }
    if (client_fd < 0) {
      printf("server acccept failed...\n");
      exit(0);
    } else
      printf("Server acccept the client...\n");
    puts("Handler assigned");
    no_threads++;
  }
  int k = 0;
  for (k = 0; k < 3; k++) {
    pthread_join(threads[k], NULL);
  }
  return 0;
}
