#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <util.h>
#include <shell.h>
#include <cmd_core.h>
#include <cmd_vmeio.h>
#include <regs_parse.h>

#define VMEIO_SHELL_VERSION "1.0"
#define BUFSIZE 1024

#define ERROR "error: "
#define WARN "warning: "

int cexec(char *buffer)
{
	int ac;
	char **av;
	int run = 0;
	struct shell_cmd *cmd;
	av = parse_args(buffer, &ac);
	if (ac < 1) {
		printf("no command given\n");
		free_args(av);
		return 0;;
	}
	/* look for the command by name and char name */
	cmd = get_command(av[0]);
	if (!cmd && strlen(av[0]) == 1)
		cmd = get_command_char(av[0][0]);
	if (!cmd) {
		printf("%s: command not found\n", av[0]);
		free_args(av);
		return 0;
	}
	/* if a command returns non-zero, it means we should exit */
	run = cmd->cmd(ac, av);
	free_args(av);

	return run;
}

int do_script(char *filename)
{
	FILE *f;
	int run;
	char *str;
	char buffer[BUFSIZE];
	f = fopen(filename, "r");
	if (!f) {
		printf("failed to open vmeiosh script: %s\n", filename);
		return 1;
	}
	while (run && fgets(buffer, sizeof(buffer), f) != NULL) {
		/* get rid of trailing newline if any */
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		/* run the commands in the line */
		str = strtok(buffer, ";");
		run = !cexec(str);
		while ((str = strtok(NULL, ";")) != NULL)
			run = !cexec(str);
	}
	fclose(f);
	return 0;
}

int main(int argc, char *argv[])
{
	int run = 1;
	char buffer[BUFSIZE];
	char *str;

	add_command("help", 'h', "Print this help information", &cmd_help, NULL, NULL);
	add_command("quit", 'q', "Exit from vmeio test shell", &cmd_quit, NULL, NULL);
	add_command("sh", 'b', "Run a shell command (b = bash)", &cmd_sh, NULL, NULL);
	add_command("write", 'w', "Write data to a device's memory", &cmd_vmeio_write, NULL, NULL);
	add_command("read", 'r', "Read data from a device's memory", &cmd_vmeio_read, NULL, NULL);
	add_command("get", 'g', "Get a device parameter", &cmd_vmeio_get, NULL, NULL);
	add_command("set", 's', "Set a device parameter", &cmd_vmeio_set, NULL, NULL);
	add_command("open", 'o', "Open a device by LUN", &cmd_vmeio_open, NULL, NULL);
	add_command("driver", 'd', "Set driver name to use", &cmd_vmeio_driver, NULL, NULL);

	/* we have a script being called */
	int i;
	if (argc > 1) {
		for (i = 1; i < argc; i++)
			do_script(argv[i]);
		return 0;
	}

	/* interactive mode */
	printf("VMEIO Test Shell (Version %s)\n", VMEIO_SHELL_VERSION);
	/* try opening .vmeiosh file for initial commands */
	do_script(".vmeiorc");
	/* go into shell loop */
	do {
		printf("> ");
		fgets(buffer, BUFSIZE - 1, stdin);
		/* get rid of trailing newline if any */
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		/* run the commands */
		str = strtok(buffer, ";");
		run = !cexec(str);
		while ((str = strtok(NULL, ";")) != NULL)
			run = !cexec(str);
	} while (run);
	free_commands();
	return 0;
}
