#ifndef __VMEIO_TEST_CMD_VMEIO_H
#define __VMEIO_TEST_CMD_VMEIO_H

int cmd_vmeio_open(int argc, char *argv[]);
int cmd_vmeio_driver(int argc, char *argv[]);
int cmd_vmeio_set(int argc, char *argv[]);
int cmd_vmeio_version(int argc, char *argv[]); /* replace with 'get' command? */

#endif
