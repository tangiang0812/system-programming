#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int dir_name_flag = 0;
  int file_name_flag = 0;
  char *dir_name = NULL;
  char *file_name = NULL;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt(argc, argv, "d:n:c:")) != -1)
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

  printf("Directory name = %s, File name = %s, cvalue = %s\n", dir_name,
         file_name, cvalue);

  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);
  return 0;
}
