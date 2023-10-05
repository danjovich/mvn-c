#if !defined(DISP_H_)
#define DISP_H_

#include <stdint.h>
#include <stdio.h>

/**
 * @brief Possible IO devices types.
 *
 */
typedef enum { keyboard, monitor, printer, disk } disp_type;

/**
 * @brief Mode (e for write, l for read) for disk type devices.
 *
 */
typedef enum { e, l } file_mode;

/**
 * @brief Type that describes the properties of an IO device.
 *
 */
typedef struct disp {
  /**
   * @brief The type of the device.
   *
   */
  disp_type type;

  /**
   * @brief Logic unit to refer to the device.
   *
   */
  uint8_t logic_unit;

  /**
   * @brief Mode for files.
   *
   */
  file_mode mode;

  /**
   * @brief The name of the file or printer, null or empty
   * if it is not of either of these types.
   *
   */
  char *filename;

  /**
   * @brief If type is disk, the file object will be in this property.
   *
   */
  FILE *file;
} Disp;

extern int disp_lst_len;
extern Disp *disp_lst;

void init_disp_list();

int fill_disp_lst(FILE *file);

#endif // DISP_H_
