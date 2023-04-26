#include "implementations/disp.c"
#include "mvn.c"

#include <stdio.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Error: No file was passed as argument\n");
    return 1;
  }

  init_disp_list();

  FILE *io = fopen("disp.lst", "r");

  if (io) {
    if (fill_disp_lst(io)) {
      return 1;
    }

    printf("disp.lst loaded\n");
  }

  FILE *file = fopen(argv[1], "r");
  if (initialize_memory(file)) {
    return 1;
  }

  mvn();

  return 0;
}
