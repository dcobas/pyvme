#ifndef __VMEIO_TEST_SHELL_H
#define __VMEIO_TEST_SHELL_H

typedef int (*command_t)(int, char **);
typedef int (*init_t)();
typedef void (*exit_t)();

struct shell_cmd {
	char *name;
	char sname;
	char *desc;
	command_t cmd;
	init_t init;
	exit_t exit;
	struct shell_cmd *next;
};

extern struct shell_cmd *cmd_list;

int add_command(char *name, char sname, char *desc, command_t cmd);
struct shell_cmd *get_command(char *name);
struct shell_cmd *get_command_char(char name);
void free_commands();

#endif
