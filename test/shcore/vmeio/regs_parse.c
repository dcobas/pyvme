#include <regs_parse.h>
#include <util.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

struct vmeio_regs *regs = NULL;

int add_reg(char *name, char *flags, char *off, unsigned int sz, unsigned int win, int depth)
{
	struct vmeio_regs *t;
	t = malloc(sizeof(struct vmeio_regs));
	if (!t)
		return -1;
	if (strlen(name) > sizeof(t->name))
		return -1;
	strcpy(t->name, name);
	if (strcmp(flags, "RO") == 0) {
		t->flags = READ;
	} else if (strcmp(flags, "WO") == 0) {
		t->flags = WRITE;
	} else if (strcmp(flags, "RW") == 0) {
		t->flags = READ | WRITE;
	} else {
		free(t);
		return -1;
	}
	t->offset = strtoul(off, NULL, 16);
	t->size = sz;
	t->window = win;
	t->depth = depth;
	t->next = regs;
	regs = t;
	return 0;
}

void free_regs()
{
	struct vmeio_regs *r;
	r = regs;
	while (r) {
		struct vmeio_regs *tmp = r;
		r = r->next;
		free(tmp);
	}
}

char *strrstr(char *s1, char *s2) {
	char *sc2, *psc1, *ps1;

	if (*s2 == '\0')
		return((char *)s1);
		
	ps1 = s1 + strlen(s1);
	
	while (ps1 != s1) {
		--ps1;
		for (psc1 = ps1, sc2 = s2; ; )
			if (*(psc1++) != *(sc2++))
				break;
			else if (*sc2 == '\0')
				return ((char *)ps1);
	}
	return ((char *)NULL);
}

int parse_regs(char *drv_name)
{
	DIR *dir;
	struct dirent *dp;
	char *dir_path = ".";
	char buffer[1024];
	dir = opendir(dir_path);
	if (!dir) {
		printf("failed to read directory\n");
		return -1;
	}
	while ((dp = readdir(dir)) != NULL) {
		if (strrstr(dp->d_name, ".regs") == NULL)
			continue;
		FILE *f = fopen(dp->d_name, "r");
		if (!f)
			continue;
		while (fgets((char *)&buffer, 1024, f) != NULL) {
			int argc;
			char **argv;
			argv = parse_args(buffer, &argc);
			if (argc < 1)
				continue;
			if (strcmp(argv[0], "NAME") == 0) {
				printf("Regs name: %s\n", argv[1]);
				continue;
			}
			else if (strcmp(argv[0], "$") == 0) {
				if (argc != 7)
					continue;
				printf("Reg:\n\tName: %s\n\tFlags: %s\n\tOff: %s\n\tSize: %d bytes\n\tWindow: %d\n", argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
			}
			free_args(argv);
		}
	}
	closedir(dir);
	return 1;
}
