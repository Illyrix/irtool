#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "irtool.h"

struct ir_filename *files;
int ir_translate = IR_OPTION_ENABLED;
int ir_diff = IR_OPTION_ENABLED;
int ir_help = IR_OPTION_DISABLED;
int ir_version = IR_OPTION_DISABLED;

char cc[MIN_IR_BUFF_LEN] = "clang";
char cobc[MIN_IR_BUFF_LEN] = "cobc";

char error_append[MAX_IR_MSG_LENGTH/2] = "\0";
struct error_code error_table[] = {
  #ifdef ERROR_ITEM
  #undef ERROR_ITEM
  #endif 
  #define ERROR_ITEM(code, error) {code, error},
  #include "error.def"
  #undef ERROR_ITEM

  {0, NULL}
};

int main (int argc, char *argv[]) {
  int code = 0;
  if ((code = command_line(argc, argv)) || (code = process())) {
    ir_error(code);
    return 1;
  }

  if (ir_diff == IR_OPTION_ENABLED || ir_translate == IR_OPTION_ENABLED)
    printf("Complete\n");

  return 0;
}

void ir_error(int code) {
  if (!code) return;

  // contains all information
  char info[MAX_IR_MSG_LENGTH];
  int i = 0;
  for (; error_table[i].code; i++) {
    if (error_table[i].code == code) {
      strcpy(info, error_table[i].error);
      if (strlen(error_append)) {
        strcat(info, error_append);
      }
      break;
    }
  }

  // error code not found
  if (! error_table[i].code) {
    if (sprintf(info, "undefined error code: %d", code) < 0) {
      printf("error occurred on sprintf()");
      return;
    }
  }
  printf("%s\n", info);
}