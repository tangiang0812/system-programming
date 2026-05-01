/**
 * C program to list all files and sub-directories in a directory.
 */

#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void listFilesRecursively(const char *path, const char *file_name);

int main(int argc, char **argv) {

  // Input path from user
  char *dir_name = NULL;
  char *file_name = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt(argc, argv, "d:n:")) != -1)
    switch (c) {
    case 'd':
      dir_name = optarg;
      break;
    case 'n':
      file_name = optarg;
      break;
    case '?':
      if (optopt == 'd')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (optopt == 'n')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }

  printf("Directory name = %s, File name = %s\n", dir_name, file_name);

  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  listFilesRecursively(dir_name, file_name);

  return 0;
}

/**
 * Lists all files and sub-directories at given path.
 */
void listFilesRecursively(const char *base_path, const char *file_name) {
  char path[1000];
  struct dirent *dp;
  DIR *dir = opendir(base_path);

  // Unable to open directory stream
  if (!dir)
    return;

  while ((dp = readdir(dir)) != NULL) {
    if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {

      // Construct new path from our base path
      strcpy(path, base_path);
      strcat(path, "/");
      strcat(path, dp->d_name);

      if (strcmp(dp->d_name, file_name) == 0)
        printf("%s\n", path);

      listFilesRecursively(path, file_name);
    }
  }

  closedir(dir);
}
