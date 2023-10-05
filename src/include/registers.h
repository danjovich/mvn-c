#if !defined(REGISTERS_H_)
#define REGISTERS_H_

#include <stdint.h>

/**
 * @brief Structure containing the 7 MVN registers.
 *
 */
typedef struct registers {
  // Memory Address Register, it saves the address to be got from the memory
  uint16_t MAR;

  // Memory Data Register, it saves the value returned from the memory
  uint16_t MDR;

  // Instruction Register, it saves the instruction got at the beginning of each
  // cycle
  uint16_t IR;

  // OPeration, it saves the operation to be executed at the cycle
  uint16_t OP;

  // Operand Instruction, it saves the operand of the instruction
  uint16_t OI;

  // ACcumulator, register that is used to save values gotten from various
  // places
  uint16_t AC;

  // Instruction Counter, it is used to save the address of the next instruction
  uint16_t IC;
} Registers;

extern Registers regs;

#endif // REGISTERS_H_
