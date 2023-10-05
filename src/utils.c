#include "types.h"

bool is_hexadecimal(char digit) {
  return (digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f') ||
         (digit >= 'A' && digit <= 'F');
}

unsigned int hexadecimal_to_int(char digits[], int digits_len) {
  unsigned int result = 0;

  for (int i = digits_len - 1; i >= 0; i--) {
    if (digits[i] >= '0' && digits[i] <= '9') {
      result += digits[i] - '0';
    } else if (digits[i] >= 'a' && digits[i] <= 'f') {
      result += digits[i] - 'a' + 10;
    } else if (digits[i] >= 'A' && digits[i] <= 'F') {
      result += digits[i] - 'A' + 10;
    }
  }

  return result;
}
