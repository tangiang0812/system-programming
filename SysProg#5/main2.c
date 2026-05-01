#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getPPID(const char *pid) {
  char path[256];
  snprintf(path, sizeof(path), "/proc/%s/stat", pid);

  FILE *file = fopen(path, "r");
  if (!file) {
    perror("Error opening process stat file");
    return -1;
  }

  int ppid;

  if (fscanf(file, "%*d %*s %*c %d", &ppid) != 1) {
    printf("fscan return %d\n", fscanf(file, "%*d %*s %*c %d", &ppid));
    perror("Error reading process stat file");
    fclose(file);
    return -1;
  }

  fclose(file);
  return ppid;
}

void printProcessInfo(const char *pid) {
  char path[256];
  snprintf(path, sizeof(path), "/proc/%s/cmdline", pid);

  FILE *file = fopen(path, "r");
  if (file) {
    char cmdline[256];
    size_t bytesRead = fread(cmdline, 1, sizeof(cmdline) - 1, file);
    fclose(file);
    int ppid;

    if (bytesRead > 0) {
      cmdline[bytesRead] = '\0';
      printf("------------------\n");
      printf("Process ID: %s\n", pid);
      printf("Command Line: %s\n", cmdline);
      ppid = getPPID(pid);
      printf("Parent Process ID: %d\n", ppid);
      printf("------------------\n");
    }
  }
}

int main() {
  DIR *dir = opendir("/proc");
  if (!dir) {
    perror("Error opening /proc directory");
    return 1;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR) {
      // Check if the directory entry is a numeric directory (process ID)
      if (atoi(entry->d_name) > 0) {
        printProcessInfo(entry->d_name);
      }
    }
  }

  closedir(dir);

  return 0;
}
