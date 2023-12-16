#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1025
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM        0
#define CMD_OR          1
#define CMD_AND         2
#define CMD_CHAIN       3

/* for convert_number() */
#define CONVERT_LOWERCASE       1
#define CONVERT_UNSIGNED         2

/* 1 if using ayatem getline () */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number failed
 * @str: a string
 * @next: points to the next node
 */

typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguements
 * @argv: an array of string generated from arg
 * @path: a string path for the current command
 * @argc: the arguement count
 * @line_count: the error count 
 * @err_num: the error code for exit () s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */

typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory managment */
	int cmd_buf_type; 
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* toem_shloop.c */
int _hsh(info_t *, char **);
int findbltin(info_t *);
void _findcmd(info_t *);
void _forkcmd(info_t *);

/* toem_parser.c */
int _iscmd(info_t *, char *);
char *_dupchar(char *, int, int);
char *findpath(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors.c */
void eputs(char *);
int eput_char(char);
int put_fd(char c, int fd);
int puts_fd(char *str, int fd);

/* toem_string.c */
int str_len(char *);
int str_cmp(char *, char *);
char *startwith(const char *, const char *);
char *str_cat(char *, char *);

/* toem_string1.c */
char *str_cpy(char *, char *);
char *str_dup(const char *);
void puts_char(char *);
int _putchar(char);

/* toem_exits.c */
char *str_cpy(char *, char *, int);
char *str_cat(char *, char *, int);
char *str_chr(char *, char);

/* toem_tokenizer.c */
char **str_tow(char *, char *);
char **str_tow2(char *, char);

/* toem_realloc.c */
char *mem_sets(char *, char, unsigned int);
void f_free(char **);
void *realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int _bfree(void **);

/* toem_atoi.c */
int interact(info_t *);
int is_delimit(char, char *);
int _isalphabet(int);
int atoi_conv(char *);

/* toem_errors1.c */
int err_atoi(char *);
void _printerr(info_t *, char *);
int printd(int, int);
char *conv_num(long int, int, int);
void rem_comm(char *);

/* toem_builtin.c */
int exit(info_t *);
int cd(info_t *);
int help(info_t *);

/* toem_builtin1.c */
int history(info_t *);
int my_ali(info_t *);

/* toem_getline.c */
ssize_t _getinput(info_t *);
int get_line(info_t *, char **, size_t *);
void sig_int_handler (int);

/* toem_getinfo.c */
void _clearinfo(info_t *);
void _setinfo(info_t *, char **);
void _freeinfo(info_t *, int);

/* toem_environ.c */
char *get_the_env(info_t *, const char *);
int _env(info_t *);
int set_env(info_t *);
int unset_env(info_t *);
int _envlist(info_t *);

/* toem_getenv.c */
char **_getenv(info_t *);
int unset_env(info_t *, char *);
int set_env(info_t *, char *, char *);

/* toem_history.c */
char *_gethist_file(info_t *info);
int _writehist(info_t *info);
int _readhist(info_t *info);
int build_hist_list(info_t *info, char *buf, int linecount);
int renum_hist(info_t *info);

/* toem_lists.c */
list_t *_addnode(list_t **, const char *, int);
list_t *node_to_end(list_t **, const char *, int);
size_t print_liststr(const list_t *);
int delnode_index(list_t **, unsigned int);
void _feelist(list_t **);

/* toem_lists1 */
size_t _listlen(const list_t *);
char **_liststr(list_t *);
size_t _printlist(const list_t *);
list_t *_nodestart(list_t *, char *, char);
ssize_t _getnodeindex(list_t *, list_t *);

/* toem_vars.c */
int ischains(info_t *, char *, size_t *);
void check_the_chain(info_t *, char *, size_t *, size_t, size_t);
int _repl_alias(info_t *);
int _repl_vars(info_t *);
int _repl_str(char **, char *);

#endif



