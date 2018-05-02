#ifndef NULL
#define NULL 0
#endif

#define IR_VERSION "0.0.1"

#define MAX_IR_MSG_LENGTH 65535
#define MEDUIM_IR_BUFF_LEN 8192
#define MIN_IR_BUFF_LEN 64

#define IR_OPTION_ENABLED 1
#define IR_OPTION_DISABLED 0

#define error_return(func, ...) {                   \
  int __res;                                        \
  if (__res = (func)(__VA_ARGS__)) return __res;    \
}

struct ir_filename {
  short use_soruce;
  char* source;
  char* ir_code;
  struct ir_filename* next;
};

struct error_code {
  int code;
  char* error;
};

extern int command_line(int, char **);
extern int process();
void ir_error(int);