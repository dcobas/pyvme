/**
 * =================================================
 * Implement driver to do basic VME IO
 * Parameters file for driver instance
 * Julian Lewis BE/CO/HT Tue 19th October 2010
 *
 * You have to fill in these macros to make your driver and support library
 * Replace the string vmeio with your driver name
 *
 * Driver configuration definitions.
 * It is supposed that this driver will be compiled under different names
 * and that nodes will be created in /dev for each named kernal module.
 * These definitions must be set up according to a particular useage.
 * DRV_NAME:           is used when printing messages
 * DRV_SYMB:           entry points symbol value
 * DRV_MODULE_VERSION: may change with new features and bug corrections
 * DRV_MAX_DEVICES:    is the number of modules you expect to handle
 */

#ifndef DRV_NAME

#define DRV_NAME "gfas"
#define DRV_SYMB(x) gfas##x
#define DRV_MODULE_VERSION "1.0"
#define DRV_MAX_DEVICES 32

#endif
