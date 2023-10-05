#if !defined(MEMORY_H_)
#define MEMORY_H_

// 4.095 kB memory
#define MEMORY_SIZE 0xFFF
#include <stdint.h>

void get_from_memory();
void set_memory(uint16_t value);

extern uint8_t memory[MEMORY_SIZE];

#endif // MEMORY_H_
