#include "process.h"
#include "irtool.h"
#include <stdio.h>
#include <stdlib.h>

static char cmd[MEDUIM_IR_BUFF_LEN];

int process() {

  error_return(verify_input)

  if (ir_help == IR_OPTION_ENABLED) {
    print_usage();
  }

  if (ir_version == IR_OPTION_ENABLED) {
    print_version();
  }

  if (ir_translate == IR_OPTION_ENABLED) {
    struct ir_filename *f = files;
    while (f->next) {
      error_return(process_translate, f)
      f = f->next;
    }
    error_return(process_translate, f)
  }

  if (ir_diff == IR_OPTION_ENABLED) {
    error_return(process_diff, files->ir_code, (*files->next).ir_code)
  }

  return 0;
}

int process_translate(struct ir_filename* f) {
  cmd[0] = '\0';
  #ifdef _MSC_VER     // for ms compiler, llvm-ir unsupported
    strcpy(error_append, " : MS C compiler unable to generate LLVM-IR");
    return 5;
  #else
    if (f->use_soruce) {
      sprintf(cmd, "%s -v -S -A \"-lcob -emit-llvm -o '%s'\" %s %s", cobc, f->ir_code, f->source, cobc_flags);
      int res;
      if (res = system(cmd)) {
        sprintf(error_append, " : execute command `%s` return %d", cmd, res);
        return 1;
      }
    }
  #endif
  return 0;
}

int process_diff(char* file_a, char* file_b) {
  cmd[0] = '\0';
  sprintf(cmd, "diff %s %s %s", file_a, file_b, diff_flags);

  // diff returned non-zero if a is not same as b 
  system(cmd);
  return 0;
}

int verify_input() {

  // need input files
  if (! files && (ir_translate == IR_OPTION_ENABLED || ir_diff == IR_OPTION_ENABLED)) {
    sprintf(error_append, " : no input file");
    return 3;
  }

  if (ir_translate == IR_OPTION_DISABLED && 
    ir_diff == IR_OPTION_DISABLED &&
    ir_help == IR_OPTION_DISABLED &&
    ir_version == IR_OPTION_DISABLED) {
    sprintf(error_append, " : translation and diff require at least one");
    return 3;
  }

  if (ir_translate == IR_OPTION_DISABLED && files) {
    struct ir_filename* fn;
    for (fn = files; fn->next; fn = fn->next) {
      if (fn->use_soruce) {
        sprintf(error_append, " : translation has been disabled; %s was input", fn->source);
        return 3;
      }
    }
  }

  if (ir_diff == IR_OPTION_ENABLED) {
    struct ir_filename* fn;
    int count = 1; 
    for (fn = files; fn->next; fn = fn->next) {
      count ++;
    }
    if (count != 2) {
      sprintf(error_append, " : input correctly 2 files as diff is enabled");
      return 3;
    }
  }
  if (ir_diff == IR_OPTION_DISABLED && files) {
    struct ir_filename* fn;
    for (fn = files; fn->next; fn = fn->next) {
      if (! fn->use_soruce) {
        sprintf(error_append, " : only receive COBOL source code as diff is disabled");
        return 3;
      }
    }
  }

  return 0;
}

void print_usage() {
  #define _(s) puts(s);
  _("Usage: irtool [options] file...")
  _("Options:")
  _("   --help, -h                      Display this message")
  _("   --only-translate, -t            Only translate COBOL code to LLVM-IR")
  _("   --only-diff, -d                 Only compare two LLVM-IR files and display their difference")
  _("   --compiler, -c <target>         Use <target> compiler execute file as COBOL compiler")
  _("   --version, -v                   Show current version")
  _("   --diff-argument <options>       Add <options> to the diff command")
  _("   --translate-argument <options>  Add <options> to the COBOL compiler phase")
  _("Files need to have extensions with .cob(for COBOL) or .ll(for LLVM-IR)")
  #undef _
}

void print_version() {
  #ifdef  IR_VERSION
    puts("IRTOOL: a tool for translating COBOL to llvm and comparing IR code");
    printf("Version: %s\n", IR_VERSION);
  #endif
}