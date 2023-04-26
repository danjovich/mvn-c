#if !defined(UTILS_C_)
#define UTILS_C_
#include "../include/types.h"

/**
 * @brief Verifies if a char is a hexadecimal digit.
 *
 * @param digit the character to be checked
 * @return true (1)
 * or
 * @return false (0)
 */
bool is_hexadecimal(char digit) {
  return (digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f') ||
         (digit >= 'A' && digit <= 'F');
}

/**
 * @brief Converts a hexadecimal number represented as a string to an unsigned
 * integer.
 *
 * @param digits the string with hexadecimal digits
 * @param digits_len length of digits
 * @return unsigned int
 */
unsigned int hexadecimal_to_int(char digits[], int digits_len) {
  unsigned int result = 0;

  for (int i = digits_len - 1; i >= 0; --i) {
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

#endif // UTILS_C_
