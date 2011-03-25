#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shell.h>
#include <cmd_core.h>

/* Command Template:
int f(int argc, char *argv[])
{
}
*/

int cmd_help(int argc, char *argv[])
{
	struct shell_cmd *c;
	c = cmd_list;
	printf("Available commands:\n");
	while (c) {
		printf("%-20s %s\n", c->name, c->desc);
		c = c->next;
	}
	return 0;
}

int cmd_quit(int argc, char *argv[])
{
	printf("Quit!\n");
	return 1;
}

int cmd_sh(int argc, char *argv[])
{
	int i;
	char buffer[1024];
	if (argc < 2) {
		printf("%s: usage: sh COMMAND\n", argv[0]);
		return 0;
	}
	strcpy(buffer, argv[1]);
	strcat(buffer, " ");
	for (i = 2; i < argc; i++) {
		strcat(buffer, argv[i]);
		strcat(buffer, " ");
	}
	system(buffer);
	return 0;
}
