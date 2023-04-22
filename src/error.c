#if !defined(ERROR_C)
#define ERROR_C

#include <stdio.h>

void error(char symbol, int position) {
  printf("Error: invalid symbol '%c' at position %d\n", symbol, position);
}

#endif // ERROR_C
