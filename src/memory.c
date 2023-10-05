#include "memory.h"

#include "registers.h"

#include <stdio.h>

uint8_t memory[MEMORY_SIZE] = {0x00};

void get_from_memory() {
  regs.MDR = (memory[regs.MAR] * 0x100) + memory[regs.MAR + 0x1];
}

void set_memory(uint16_t value) {
  memory[regs.MAR] = value / 0x100;
  memory[regs.MAR + 0x1] = value;
}
