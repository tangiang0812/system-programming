#include "stdio.h"
#include <unistd.h>

int main() {
  printf("Hello World! - PID:%d - PPID:%d\n", getpid(), getppid());
  for (;;) {
  }
  return 0;
}
