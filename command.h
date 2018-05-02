extern struct ir_filename *files;
extern int ir_translate;
extern int ir_diff;
extern int ir_help;
extern int ir_version;
extern char error_append[];
extern char cc[];
extern char cobc[];

int command_line(int, char **);
int get_command_args(int, char **);
static void file_basename (const char *, char *);
static const char* file_extension (const char *);