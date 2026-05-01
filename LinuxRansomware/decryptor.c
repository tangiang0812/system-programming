
#include <ctype.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char key = 'a';
void xorEncrypt(const char *path);

int main(int argc, char **argv) {

  // Input path from user
  char *dir_name = "c";
  char *file_name = NULL;
  char *path = NULL;
  int index;
  int c;
  opterr = 0;

  while ((c = getopt(argc, argv, "n:")) != -1)
    switch (c) {
    case 'n':
      file_name = optarg;
      break;
    case '?':
      if (optopt == 'n')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }
  xorEncrypt(file_name);

  return 0;
}

void xorEncrypt(const char *path) {
  char encrypted_filename[PATH_MAX], ch;
  FILE *fpts, *fptt;

  char suffix[11] = ".decrypted";

  strncpy(encrypted_filename, path, PATH_MAX);
  strncat(encrypted_filename, suffix, sizeof(suffix));

  fpts = fopen(path, "r");
  if (fpts == NULL) {
    printf(" File does not exists or error in opening..!!");
    return;
  }
  fptt = fopen(encrypted_filename, "w");
  if (fptt == NULL) {
    printf(" Error in creation of file temp.txt ..!!");
    fclose(fpts);
    return;
  }
  while (1) {
    ch = fgetc(fpts);
    if (ch == EOF) {
      break;
    } else {
      ch = ch ^ key;
      fputc(ch, fptt);
    }
  }
  fclose(fpts);
  fclose(fptt);
}
