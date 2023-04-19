#if !defined(LAU_H_)
#define LAU_H_
#include "types.h"

#include <stdint.h>

/**
 * @brief Checks if a value is 0.
 *
 * @param v the value to check
 * @return true if v == 0, else false
 */
bool is_zero(int16_t v);

/**
 * @brief Checks if a value is lower than 0.
 *
 * @param v the value to check
 * @return true if v < 0, else false
 */
bool is_neg(int16_t v);

/**
 * @brief Sums two values.
 *
 * @param a
 * @param b
 * @return a + b
 */
int16_t add(int16_t a, int16_t b);

/**
 * @brief Subtracts two values.
 *
 * @param a
 * @param b
 * @return a - b
 */
int16_t sub(int16_t a, int16_t b);

/**
 * @brief Multiplies two values.
 *
 * @param a
 * @param b
 * @return a * b
 */
int16_t mul(int16_t a, int16_t b);

/**
 * @brief Divides two values.
 *
 * @param a
 * @param b
 * @return a / b
 */
int16_t div(int16_t a, int16_t b);

#endif // LAU_H_
