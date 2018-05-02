extern struct ir_filename *files;
extern int ir_translate;
extern int ir_diff;
extern int ir_help;
extern int ir_version;
extern char error_append[];
extern char cc[];
extern char cobc[];
extern char diff_flags[];
extern char cobc_flags[];

int process();

int verify_input();

int process_translate(struct ir_filename*);
int process_diff(char*, char*);
void print_usage();
void print_version();