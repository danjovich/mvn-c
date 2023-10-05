#include "error.h"
#include <stdio.h>

void error(char symbol, int position) {
  printf("Error: invalid symbol '%c' at position %d\n", symbol, position);
}
