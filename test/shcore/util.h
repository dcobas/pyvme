#ifndef VMEIO_UTIL_H
#define VMEIO_UTIL_H

int setargs(char *args, char **argv);
char **parse_args(char *args, int *argc);
void free_args(char **argv);

#endif
