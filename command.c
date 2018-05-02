#include "command.h"
#include "irtool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#include <getopt.h>
#endif

static const struct option long_option[] = {
  #ifndef OPT_ITEM
  #define OPT_ITEM(x, y, z, w) {x, y, z, w},
  #endif
  #include "opt.def"
  #undef OPT_ITEM
  {NULL, 0, NULL, 0}
};

char diff_flags[MEDUIM_IR_BUFF_LEN] = "\0";
char cobc_flags[MEDUIM_IR_BUFF_LEN] = "\0";
int files_num = 0;

int command_line(int argc, char **argv) {
  // adapt Windows style command args
  for(int i = 1; i < argc; i++) {
    // strrchr returns a char pointer address
    // avoid using argv[i][0] directly
    if (strrchr(argv[i], '/') == argv[i]) {
      argv[i][0] = '-';
    }
  }

  return get_command_args(argc, argv);
}

int get_command_args(int argc, char **argv){
  int res = 0;

  while ((res = getopt_long(argc, argv, "vthdc:", long_option, NULL)) != -1) {
    switch (res) {
      case 't':
        ir_translate = IR_OPTION_ENABLED;
        ir_diff = IR_OPTION_DISABLED;
        break;
      case 'd':
        ir_translate = IR_OPTION_DISABLED;
        ir_diff = IR_OPTION_ENABLED;
        files_num = 2;                // only allow 2 input file
        break;
      case 'c':
        strcpy(cobc, optarg);
        break;
      case 'A': // compile arg
        strcpy(cobc_flags, optarg);
        break;
      case 'a': // diff arg
        strcpy(diff_flags, optarg);
        break;
      case 'h':
        ir_help = IR_OPTION_ENABLED;
        ir_translate = IR_OPTION_DISABLED;
        ir_diff = IR_OPTION_DISABLED;
        break;
      case 'v':
        ir_translate = IR_OPTION_DISABLED;
        ir_diff = IR_OPTION_DISABLED;
        ir_version = IR_OPTION_ENABLED;
        break;
      case '?':
        sprintf(error_append, " : unknown option \"-%c\", use irtool -h to get usage", optopt);
        return 3;
      default :
        sprintf(error_append, " : unknown error in parsing args");
        return 3;
    }
  }
  // get no-option args (filenames)
  char buff[MIN_IR_BUFF_LEN] = "\0";
  for (int i = optind; i < argc; i++) {
    struct ir_filename* temp = malloc(sizeof(struct ir_filename));

    file_basename(argv[i], buff);
    const char* ext  = file_extension(argv[i]);

    if (0 == strcmp(ext, "cob")) {     // .cob
      temp->source = argv[i];
      temp->ir_code = malloc(strlen(strcat(buff, ".ll")) + 1);
      sprintf(temp->ir_code, "%s", buff);
      temp->use_soruce = 1;
      temp->next = NULL;
    }

    if (0 == strcmp(ext, "ll")) {     // .ll
      temp->source = NULL;
      temp->ir_code = argv[i];
      temp->use_soruce = 0;
      temp->next = NULL;
    }

    if (strcmp(ext, "ll") && strcmp(ext, "cob")) {   // not .ll nor .cob
      sprintf(error_append, " : .%s; only receive .ll, .cob", ext);
      return 6;
    }

    if (! files) {            // add to files list
      files = temp;
    } else {
      struct ir_filename* fn;
      for (fn = files; fn->next; fn = fn->next);
      fn->next = temp;
    }
    ext = "\0";
  }

  return 0;
}

static void file_basename (const char *filename, char *buff) {
	const char	*startp;
	const char	*endp;
	unsigned int	len;

	// Remove directory name
	startp = strrchr (filename, '/');
	if (startp) {
		startp++;
	} else {
		startp = filename;
	}

	// Remove extension
	endp = strrchr (filename, '.');
	if (endp > startp) {
		len = endp - startp;
	} else {
		len = strlen (startp);
	}

	// Copy base name
	strncpy (buff, startp, len);
	buff[len] = '\0';
}

static const char* file_extension (const char *filename) {
	const char *p;

	p = strrchr (filename, '.');
	if (p) {
		return p + 1;
	}
	return "";
}
