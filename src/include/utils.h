#if !defined(UTILS_H_)
#define UTILS_H_

#include "types.h"

/**
 * @brief Verifies if a char is a hexadecimal digit.
 *
 * @param digit the character to be checked
 * @return true (1)
 * or
 * @return false (0)
 */
bool is_hexadecimal(char digit);

/**
 * @brief Converts a hexadecimal number represented as a string to an unsigned
 * integer.
 *
 * @param digits the string with hexadecimal digits
 * @param digits_len length of digits
 * @return unsigned int
 */
unsigned int hexadecimal_to_int(char digits[], int digits_len);

#endif // UTILS_H_
