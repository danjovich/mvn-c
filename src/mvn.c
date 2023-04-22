#include "error.c"
#include "implementations/lau.c"
#include "implementations/memory.c"
#include "implementations/registers.c"

#include <stdio.h>

bool execute_mvn_instruction() {
  switch (regs.OP) {
  case 0x0:
    // JP
    regs.IC = regs.OI;
    break;
  case 0x1:
    // JZ
    if (regs.AC == 0x0) {
      regs.IC = regs.OI;
    } else {
      regs.IC += 0x2;
    }
    break;
  case 0x2:
    // JN
    if (regs.AC >= 0x8000) {
      regs.IC = regs.OI;
    } else {
      regs.IC += 0x2;
    }
    break;
  case 0x3:
    // LV
    regs.AC = regs.OI;
    regs.IC += 0x2;
    break;
  case 0x4:
    // AD
    regs.MAR = regs.OI;
    get_from_memory();
    regs.AC += regs.MDR;
    regs.IC += 0x2;
    break;
  case 0x5:
    // SB
    regs.MAR = regs.OI;
    get_from_memory();
    regs.AC -= regs.MDR;
    regs.IC += 0x2;
    break;
  case 0x6:
    // ML
    regs.MAR = regs.OI;
    get_from_memory();
    regs.AC *= regs.MDR;
    regs.IC += 0x2;
    break;
  case 0x7:
    // DV
    regs.MAR = regs.OI;
    get_from_memory();
    regs.AC /= regs.MDR;
    regs.IC += 0x2;
    break;
  case 0x8:
    // LD
    regs.MAR = regs.OI;
    get_from_memory();
    regs.AC = regs.MDR;
    regs.IC += 0x2;
    break;
  case 0x9:
    // MM
    regs.MAR = regs.OI;
    set_memory(regs.AC);
    regs.IC += 0x2;
    break;
  case 0xA:
    // SC
    regs.MAR = regs.OI;
    set_memory(regs.IC + 0x2);
    regs.IC = regs.OI + 0x2;
    break;
  case 0xB:
    // RS
    regs.MAR = regs.OI;
    get_from_memory();
    regs.IC = regs.MDR;
    break;
  case 0xC:
    // HM
    regs.IC = regs.OI;
    return true;
  case 0xD:
    // GD
    regs.AC = getchar() * 0x100;
    regs.AC += getchar();
    regs.IC += 0x2;
    break;
  case 0xE:
    putchar(regs.AC / 0x100);
    putchar(regs.AC);
    regs.IC += 0x2;
    break;
  case 0xF:
    regs.IC += 0x2;
    break;
  default:
    regs.IC += 0x2;
    break;
  }

  return false;
}

void mvn() {
  while (regs.IC < MEMORY_SIZE) {
    regs.IR = ((memory[regs.IC] * 0x100) + memory[regs.IC + 0x1]);
    regs.OP = regs.IR / 0x1000;
    regs.OI = regs.IR - (regs.OP * 0x1000);

    // printf("IC: %x, OP: %x, OI: %x, AC: %x", regs.IC, regs.OP, regs.OI,
    //  regs.AC);
    // printf("%x %x\n", memory[regs.IC], memory[regs.IC + 1]);
    // printf("%x %x %x %x\n", regs.IC, regs.IR, regs.OP, regs.OI);
    // getchar();

    if (execute_mvn_instruction()) {
      return;
    }
  }
}

bool handle_hexadecimal(char start, char end, char *symbol, uint16_t *value,
                        uint16_t *multiplier, bool *ended) {
  if (*symbol >= start && *symbol <= end) {
    int add_to_symbol = 0;

    if (start != '0') {
      add_to_symbol = 10;
    }

    if (*multiplier == 0x0) {
      *multiplier = 0x1000;
    }

    *value += (*symbol - start + add_to_symbol) * (*multiplier);
    *multiplier /= 0x10;

    if (*multiplier == 0x0) {
      *ended = true;
    }

    return true;
  }

  return false;
}

int initialize_memory(FILE *file) {
  char symbol;
  int position = 0;
  uint16_t address = 0x0, instruction = 0x0, address_multiplier = 0x0,
           instruction_multiplier = 0x0;
  bool address_ended = false, instruction_ended = false,
       read_hexadecimal = false;

  while (!feof(file)) {
    position++;
    fscanf(file, "%c", &symbol);

    if (!address_ended) {
      read_hexadecimal =
          handle_hexadecimal('0', '9', &symbol, &address, &address_multiplier,
                             &address_ended) ||
          handle_hexadecimal('a', 'f', &symbol, &address, &address_multiplier,
                             &address_ended) ||
          handle_hexadecimal('A', 'F', &symbol, &address, &address_multiplier,
                             &address_ended);

      if (address_ended) {
        position++;
        fscanf(file, "%c", &symbol);
        if (symbol != ' ' && symbol != '\t') {
          error(symbol, position);
          return 1;
        }
      } else if (read_hexadecimal) {
        continue;
      }
    }

    if (!instruction_ended) {
      read_hexadecimal =
          handle_hexadecimal('0', '9', &symbol, &instruction,
                             &instruction_multiplier, &instruction_ended) ||
          handle_hexadecimal('a', 'f', &symbol, &instruction,
                             &instruction_multiplier, &instruction_ended) ||
          handle_hexadecimal('A', 'F', &symbol, &instruction,
                             &instruction_multiplier, &instruction_ended);

      if (instruction_ended) {
        position++;
        fscanf(file, "%c", &symbol);
        if (symbol != ' ' && symbol != '\t' && symbol != '\n' &&
            symbol != ';') {
          error(symbol, position);
          return 1;
        }

        memory[address] = instruction / 0x100;
        memory[address + 1] = instruction;
      } else if (read_hexadecimal) {
        continue;
      }
    }

    if (symbol == ' ' || symbol == '\t') {
      continue;
    }

    if (symbol == ';') {
      while (symbol != '\n' && !feof(file)) {
        position++;
        fscanf(file, "%c", &symbol);
      }
    }

    if (symbol == '\n') {
      address = 0x0;
      address_multiplier = 0x0;
      address_ended = false;
      instruction = 0x0;
      instruction_multiplier = 0x0;
      instruction_ended = false;
    }
  }

  return 0;
}
