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

/* buffer sizes */
#define CMD_MSG_LEN 8
#define DATA_LINE_LEN 64
#define DATA_LINES 2

/* memory map addresses */
#define CONFIGS_ADDR 0x73
#define BUTTONS_ADDR 0x72
#define MACRO_ADDR_START 0x6f
#define NUM_MACROS 19

/* bit calculations for address checks */
#define SETTINGS_ADDR_MAX 0x73
#define SETTINGS_ADDR_PARITY 0x0c
#define MACRO_ADDR_PARITY 0x70
#define MACRO_MEM_FLAG 0x0f
/* bit command flags */
#define ADDR_READ (1 << 7)


/* unknown, infrequently sent commands */
#define CMD_ADMIN_1 {0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x7d}
#define CMD_ADMIN_2 {0x01,0xfc,0x00,0x00,0x00,0x00,0x00,0x02}




/* Settings locations */
#define ANGLE_SNAP_ADDR 0x33
#define ANGLE_SNAP_ENABLED  1
#define ANGLE_SNAP_DISABLED 0
//oddly no linear or bitwise relationship here
#define ANGLE_CORRECT_ADDR 0x31
#define ANGLE_CORRECT_NEG2 0xe2
#define ANGLE_CORRECT_NEG1 0xf6
#define ANGLE_CORRECT_ZERO 0x00
#define ANGLE_CORRECT_POS1 0x0f
#define ANGLE_CORRECT_POS2 0x1e

#define LED_MODE_ADDR 0x47
#define LED_MODE_OFF     0x00
#define LED_MODE_STD     0x01
#define LED_MODE_BREATHE 0x02
#define LED_MODE_NEON    0x03

#define LED_CFG_ADDR 0x48
#define LED_BRIGHT_MIN   0x00
#define LED_BRIGHT_MAX   0x0A
#define LED_SPEED_MIN    0x01
#define LED_SPEED_MAX    0x0F

typedef int (*MXCommand)(int, char **);
#define MXCOMMAND(func_name) int func_name(int argc, char **argv)



/*
	User-facing commands
*/
MXCOMMAND(angle_snap);
MXCOMMAND(angle_correct);
MXCOMMAND(led_mode);
MXCOMMAND(led_brightness);
MXCOMMAND(led_speed);

/* setup */
int send_startup_cmds(void);

#endif
