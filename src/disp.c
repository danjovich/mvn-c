#include "disp.h"

#include "types.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int disp_lst_len = 2;
Disp *disp_lst;

void init_disp_list() {
  disp_lst = malloc(disp_lst_len * sizeof(Disp));
  disp_lst[0] = (Disp){keyboard, 0, '\0', "", NULL};
  disp_lst[1] = (Disp){monitor, 0, '\0', "", NULL};
}

int fill_disp_lst(FILE *file) {
  char read_char;

  while (!feof(file)) {
    bool read_type = false, read_lu = false, read_filename = false;

    disp_lst = realloc(disp_lst, (++disp_lst_len) * sizeof(Disp));
    Disp *curr_disp = &((Disp){0, 0, 0, malloc(1), NULL});
    fscanf(file, "%c", &read_char);
    do {
      if (read_char == ' ') {
        fscanf(file, "%c", &read_char);
        continue;
      } else if (read_char == '\n') {
        break;
      }

      if (!read_type) {
        if (read_char == '0') {
          curr_disp->type = keyboard;
        } else if (read_char == '1') {
          curr_disp->type = monitor;
        } else if (read_char == '2') {
          curr_disp->type = printer;
        } else if (read_char == '3') {
          curr_disp->type = disk;
        } else {
          free(curr_disp->filename);
          printf("Error: invalid device type\n");
          return 1;
        }

        fscanf(file, "%c", &read_char);
        if (read_char != ' ') {
          free(curr_disp->filename);
          printf("Error: invalid disp.lst file\n");
          return 1;
        }

        read_type = true;
      } else if (!read_lu) {
        bool read_newline = false;

        if (!is_hexadecimal(read_char)) {
          free(curr_disp->filename);
          printf("Error: invalid disp.lst file\n");
          return 1;
        }

        unsigned int lu = hexadecimal_to_int(&read_char, 1);

        if (!feof(file)) {
          fscanf(file, "%c", &read_char);

          if (read_char == '\n') {
            read_newline = true;
          } else if (is_hexadecimal(read_char)) {
            lu += hexadecimal_to_int(&read_char, 1);
            fscanf(file, "%c", &read_char);

            if (read_char == '\n') {
              read_newline = true;
            } else if (read_char != ' ') {
              free(curr_disp->filename);
              printf("Error: invalid disp.lst file\n");
              return 1;
            }
          } else if (read_char != ' ') {
            free(curr_disp->filename);
            printf("Error: invalid disp.lst file\n");
            return 1;
          }
        }

        if (read_newline &&
            (curr_disp->type == disk || curr_disp->type == printer)) {
          free(curr_disp->filename);
          printf("Error: invalid disp.lst file\n");
          return 1;
        }

        curr_disp->logic_unit = lu;
        read_lu = true;

        if (read_newline) {
          break;
        }
      } else if (!read_filename) {
        if (curr_disp->type == keyboard || curr_disp->type == monitor) {
          free(curr_disp->filename);
          printf("Error: invalid disp.lst file\n");
          return 1;
        }

        int filename_size = 0;
        while (!feof(file) && read_char != ' ' && read_char != '\n') {
          curr_disp->filename = realloc(curr_disp->filename, ++filename_size);
          curr_disp->filename[filename_size - 1] = read_char;
          fscanf(file, "%c", &read_char);
        }

        read_filename = true;

        if (curr_disp->type == printer) {
          if (!feof(file) && read_char != '\n') {
            free(curr_disp->filename);
            printf("Error: invalid disp.lst file\n");
            return 1;
          }

          break;
        }
      } else {
        if (curr_disp->type != disk || (read_char != 'e' && read_char != 'l')) {
          free(curr_disp->filename);
          printf("Error: invalid disp.lst file\n");
          return 1;
        }

        curr_disp->mode = read_char;
      }

      fscanf(file, "%c", &read_char);
    } while (read_char != '\n' && !feof(file));
    disp_lst[disp_lst_len - 1] = *curr_disp;
  }

  return 0;
}
