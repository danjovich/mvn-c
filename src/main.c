#include "mvn.c"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Error: No file was passed as argument\n");
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  if (initialize_memory(file)) {
    return 1;
  }

  mvn();

  return 0;
}
