/*
MX3100DRV - portable driver for the MX-3100 gaming mouse
Copyright (C) 2017  Dan Panzarella

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef MX_MX_H
#define MX_MX_H

#define CMD_MSG_LEN 8
#define DATA_LINE_LEN 64
#define DATA_LINES 2

#define CONFIGS_ADDR 0x73
#define BUTTONS_ADDR 0x72

#define SETTINGS_ADDR_MAX 0x73
#define SETTINGS_ADDR_PARITY 0x0c
#define MACRO_ADDR_PARITY 0x70

#define MACRO_MEM_FLAG 0x0f
#define MACRO_ADDR_START 0x6f
#define NUM_MACROS 19

/* unknown, infrequently sent commands */
#define CMD_ADMIN_1 {0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x7d}
#define CMD_ADMIN_2 {0x01,0xfc,0x00,0x00,0x00,0x00,0x00,0x02}



#define ADDR_READ (1 << 7)





typedef int (*MXCommand)(int, char **, int);
#define MXCOMMAND(func_name) int func_name(int argc, char **argv, int verbose)



/*
	User-facing commands
*/


/* internal helpers */
int send_startup_cmds(void);

int read_section(unsigned char addr, unsigned char *buf);
int write_section(unsigned char addr, unsigned char *buf);

int set_bytes(unsigned char addr, unsigned char offset, unsigned char len, unsigned char *buf);
int rewrite(unsigned char addr);

#endif
