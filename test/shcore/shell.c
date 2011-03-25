#include <shell.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct shell_cmd *cmd_list = NULL;

int add_command(char *name, char sname, char *desc, command_t cmd)
{
	struct shell_cmd *command;
	if (!name || !cmd)
		return -1;
	if (get_command(name) || get_command_char(sname))
		return -1;
	command = malloc(sizeof(struct shell_cmd));
	if (!command)
		return -1;
	command->name = malloc(strlen(name) + 1);
	if (!command->name) {
		free(command);
		return -1;
	}
	command->desc = malloc(strlen(desc) + 1);
	if (!command->desc) {
		free(command->name);
		free(command);
		return -1;
	}
	strcpy(command->name, name);
	strcpy(command->desc, desc);
	command->sname = sname;
	command->cmd = cmd;
	command->next = NULL;
	if (!cmd_list) {
		cmd_list = command;
	} else {
		command->next = cmd_list;
		cmd_list = command;
	}
	if (command->init)
		command->init();
	return 0;
}

struct shell_cmd *get_command(char *name)
{
	struct shell_cmd *command;
	command = cmd_list;
	while (command) {
		if (strcmp(command->name, name) == 0)
			return command;
		command = command->next;
	}
	return NULL;
}

struct shell_cmd *get_command_char(char sname)
{
	struct shell_cmd *command;
	command = cmd_list;
	while (command) {
		if (command->sname == sname)
			return command;
		command = command->next;
	}
	return NULL;
}


void free_commands()
{
	struct shell_cmd *command;
	struct shell_cmd *tmp;
	command = cmd_list;
	while (command) {
		tmp = command;
		command = command->next;
		if (tmp->exit)
			tmp->exit();
		if (tmp->name)
			free(tmp->name);
		if (tmp->desc)
			free(tmp->desc);
		free(tmp);
	}
}
