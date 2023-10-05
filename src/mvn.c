#include "mvn.h"

#include "disp.h"
#include "error.h"
#include "memory.h"
#include "registers.h"
#include "types.h"

int get_from_device(disp_type device_type, uint8_t logic_unit) {
  if (device_type == keyboard) {
    regs.AC = getchar() * 0x100;
    regs.AC += getchar();
  } else if (device_type == printer) {
    // TODO: implement printer connection
  } else if (device_type == disk) {
    for (int i = 0; i < disp_lst_len; i++) {
      if (disp_lst[i].type == disk && disp_lst[i].logic_unit == logic_unit) {
        if (!feof(disp_lst[i].file)) {
          regs.AC = fgetc(disp_lst[i].file) * 0x100;
          if (!feof(disp_lst[i].file)) {
            regs.AC += fgetc(disp_lst[i].file);
          } else {
            printf("No more data to get, returning 0x00\n");
            regs.AC += 0x0;
          }
        } else {
          printf("No more data to get, returning 0x00\n");
          regs.AC = 0x0;
        }
        break;
      }
    }
  } else if (device_type == monitor) {
    printf("Error: invalid device type 'monitor' for instruction 'D'\n");
    return 1;
  }

  return 0;
}

int put_on_device(disp_type device_type, uint8_t logic_unit) {
  unsigned char c1 = regs.AC / 0x100, c2 = regs.AC - (c1 * 0x100);

  if (device_type == monitor) {
    putchar(c1);
    putchar(c2);
  } else if (device_type == printer) {
    // TODO: implement printer connection
  } else if (device_type == disk) {
    for (int i = 0; i < disp_lst_len; i++) {
      if (disp_lst[i].type == disk && disp_lst[i].logic_unit == logic_unit) {
        fputc(c1, disp_lst[i].file);
        fputc(c2, disp_lst[i].file);
        break;
      }
    }
  } else if (device_type == keyboard) {
    printf("Error: invalid device type 'keyboard' for instruction 'E'\n");
    return 1;
  }

  return 0;
}

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
    if (get_from_device(regs.OI / 0x100, regs.OI)) {
      return true;
    }
    regs.IC += 0x2;
    break;
  case 0xE:
    // PD
    if (put_on_device(regs.OI / 0x100, regs.OI)) {
      return true;
    }
    regs.IC += 0x2;
    break;
  case 0xF:
    // OS
    // TODO: implement F instruction
    regs.IC += 0x2;
    break;
  default:
    regs.IC += 0x2;
    break;
  }

  return false;
}

void mvn() {
  for (int i = 0; i < disp_lst_len; i++) {
    if (disp_lst[i].type == disk) {
      char *file_mode;

      if (disp_lst[i].mode == 'l') {
        file_mode = "r";
      } else if (disp_lst[i].mode == 'e') {
        file_mode = "w";
      } else {
        file_mode = "r";
      }

      FILE *file = fopen(disp_lst[i].filename, file_mode);

      disp_lst[i].file = file;
    }
  }

  while (regs.IC < MEMORY_SIZE) {
    regs.MAR = regs.IC;
    get_from_memory();
    regs.IR = regs.MDR;
    regs.OP = regs.IR / 0x1000;
    regs.OI = regs.IR - (regs.OP * 0x1000);

    if (execute_mvn_instruction()) {
      return;
    }

    // printf("MAR: %x, MDR: %x, IC: %x, IR: %x, OP: %x, OI: %x, AC: %x\n",
    //  regs.MAR, regs.MDR, regs.IC, regs.IR, regs.OP, regs.OI, regs.AC);
    // getchar();
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
