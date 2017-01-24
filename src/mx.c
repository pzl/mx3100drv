/*
MX3100RV - portable driver for the MX-3100 gaming mouse
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memcpy */
#include "usb.h"
#include "mx.h"

#define ENFORCE_ONOFF(arg) do{\
	if (!is_on_off(arg)) {\
		fprintf(stderr, "valid values are 'on' or 'off'.\n"); return -2; \
	} } while(0)

static int valid_hex(char *s);
static int is_on_off(char *s);
static int on(char *s);

MXCOMMAND(angle_snap) {
	int err;
	unsigned char settings[DATA_LINE_LEN*DATA_LINES],
				  buttons[DATA_LINE_LEN*DATA_LINES];
	err = read_section(CONFIGS_ADDR, settings);
	if (err != 0) {
		fprintf(stderr, "Error retrieving mouse info\n");
		return -2;
	}
	err = read_section(BUTTONS_ADDR, buttons);
	if (err != 0) {
		fprintf(stderr, "Error retrieving mouse info\n");
		return -2;
	}

	if (argc == 0) {
		if (settings[ANGLE_SNAP_ADDR] == ANGLE_SNAP_ENABLED) {
			printf("on\n");
		} else {
			printf("off\n");
		}
		return 0;
	}
	ENFORCE_ONOFF(argv[0]);
	if (on(argv[0])) {
		settings[ANGLE_SNAP_ADDR] = ANGLE_SNAP_ENABLED;
	} else {
		settings[ANGLE_SNAP_ADDR] = ANGLE_SNAP_DISABLED;
	}
	err = write_section(CONFIGS_ADDR,settings);
	if (err != 0){
		fprintf(stderr, "Error changing angle snap\n");
		return err;
	}
	return write_section(BUTTONS_ADDR,buttons);
}

MXCOMMAND(angle_correct) {
	int err, angle;
	unsigned char settings[DATA_LINE_LEN*DATA_LINES],
				  buttons[DATA_LINE_LEN*DATA_LINES];
	err = read_section(CONFIGS_ADDR, settings);
	if (err != 0) {
		fprintf(stderr, "Error retrieving mouse info\n");
		return -2;
	}
	err = read_section(BUTTONS_ADDR, buttons);
	if (err != 0) {
		fprintf(stderr, "Error retrieving mouse info\n");
		return -2;
	}

	if (argc == 0) {
		switch (settings[ANGLE_CORRECT_ADDR]) {
			case ANGLE_CORRECT_NEG2: printf("-2\n"); break;
			case ANGLE_CORRECT_NEG1: printf("-1\n"); break;
			case ANGLE_CORRECT_ZERO: printf("0\n"); break;
			case ANGLE_CORRECT_POS1: printf("1\n"); break;
			case ANGLE_CORRECT_POS2: printf("2\n"); break;
			default:
				printf("unknown value: 0x%02x\n", settings[ANGLE_CORRECT_ADDR]);
				break;
		}
		return 0;
	}

	angle = atoi(argv[0]);

	switch (angle) {
		case -2: settings[ANGLE_CORRECT_ADDR] = ANGLE_CORRECT_NEG2; break;
		case -1: settings[ANGLE_CORRECT_ADDR] = ANGLE_CORRECT_NEG1; break;
		case 0: settings[ANGLE_CORRECT_ADDR] = ANGLE_CORRECT_ZERO; break;
		case 1: settings[ANGLE_CORRECT_ADDR] = ANGLE_CORRECT_POS1; break;
		case 2: settings[ANGLE_CORRECT_ADDR] = ANGLE_CORRECT_POS2; break;
		default:
			fprintf(stderr, "invalid number provided. Must be between -2 and 2. %d given\n", angle);
			return -2;
	}
	err = write_section(CONFIGS_ADDR,settings);
	if (err != 0){
		fprintf(stderr, "Error changing angle snap\n");
		return err;
	}
	return write_section(BUTTONS_ADDR,buttons);
}

MXCOMMAND(led_mode) {
	int err;
	unsigned char settings[DATA_LINE_LEN*DATA_LINES],
				  buttons[DATA_LINE_LEN*DATA_LINES];
	err = read_section(CONFIGS_ADDR, settings);
	if (err != 0) {
		fprintf(stderr, "Error retrieving mouse info\n");
		return -2;
	}
	err = read_section(BUTTONS_ADDR, buttons);
	if (err != 0) {
		fprintf(stderr, "Error retrieving mouse info\n");
		return -2;
	}

	if (argc == 0) {
		switch (settings[LED_MODE_ADDR]) {
			case LED_MODE_OFF: printf("off\n"); break;
			case LED_MODE_STD: printf("standard\n"); break;
			case LED_MODE_BREATHE: printf("breathe\n"); break;
			case LED_MODE_NEON: printf("neon\n"); break;
			default: printf("unknown value: 0x%02x\n", settings[LED_MODE_ADDR]); break;
		}
		return 0;
	}

	if (strcmp(argv[0],"off") == 0) {
		settings[LED_MODE_ADDR] = LED_MODE_OFF;
	} else if (strcmp(argv[0],"standard") == 0) {
		settings[LED_MODE_ADDR] = LED_MODE_STD;
	} else if (strcmp(argv[0],"breathe") == 0) {
		settings[LED_MODE_ADDR] = LED_MODE_BREATHE;
	} else if (strcmp(argv[0],"neon") == 0) {
		settings[LED_MODE_ADDR] = LED_MODE_NEON;
	} else {
		fprintf(stderr, "Invalid argument. Must be one of: off, standard, breathe, neon\n");
		return -2;
	}

	err = write_section(CONFIGS_ADDR, settings);
	if (err != 0) {
		fprintf(stderr, "Error changing LED mode\n");
		return err;
	}
	return write_section(BUTTONS_ADDR, buttons);
}


int read_section(unsigned char addr, unsigned char *buf) {
	unsigned char cmd[CMD_MSG_LEN] = {ADDR_READ,0x00,0x00,0x00,0x00,0x00,0x00,addr};
	unsigned char rsp[CMD_MSG_LEN];
	int err;

	if ( addr == CONFIGS_ADDR || addr == BUTTONS_ADDR ) {
		cmd[0] |= SETTINGS_ADDR_MAX-addr + SETTINGS_ADDR_PARITY;
	} else {
		cmd[0] |= MACRO_MEM_FLAG;
		cmd[1] = MACRO_ADDR_PARITY-addr;
	}

	err = send_ctl(cmd);
	if (err != 0){
		return err;
	}

	err = read_ctl(rsp);
	if (err != 0) {
		return err;
	}

	err = read_data( buf );
	err = read_data( &buf[DATA_LINE_LEN] );

	return err;
}

int write_section(unsigned char addr, unsigned char *buf) {
	unsigned char cmd[CMD_MSG_LEN] = {0x00,0x00,DATA_LINE_LEN*DATA_LINES,0x00,0x00,0x00,0x00,addr};
	int err;

	if ( addr == CONFIGS_ADDR || addr == BUTTONS_ADDR ) {
		cmd[0] = SETTINGS_ADDR_MAX-addr + SETTINGS_ADDR_PARITY;
	} else {
		cmd[0] = MACRO_MEM_FLAG;
		cmd[1] = MACRO_ADDR_PARITY-addr;
	}

	err = send_ctl(cmd);
	if (err != 0) {
		return err;
	}

	err = send_data(buf);
	err = send_data(&buf[DATA_LINE_LEN]);

	return err;
}

int set_bytes(unsigned char addr, unsigned char offset, unsigned char len, unsigned char *buf) {
	unsigned char data[DATA_LINE_LEN*DATA_LINES];
	int err;

	err = read_section(addr, data);
	if (err != 0){
		return err;
	}

	memcpy( &data[offset], buf, len );

	err = write_section(addr, data);
	return err;
}

int rewrite(unsigned char addr) {
	unsigned char data[DATA_LINE_LEN*DATA_LINES];
	int err;

	err = read_section(addr, data);
	if (err != 0){
		return err;
	}

	err = write_section(addr, data);
	return err;
}

int send_startup_cmds(void) {
	int err;

	unsigned char start1[CMD_MSG_LEN] = {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0xfd};
	unsigned char start2[CMD_MSG_LEN] = {0x03,0x00,0x02,0x00,0x00,0x00,0x00,0xfa}; /* @todo poll rate */
	err = send_ctl(start1);
	err = send_ctl(start2);
	return err;
}

static int valid_hex(char *s) {
	if (strlen(s) == 6 && strspn(s,"0123456789abcdefABCDEF") == 6){
		return 1;
	} else {
		return 0;
	}
}

static int on(char *s) {
	return s[1] == 'n';
}

static int is_on_off(char *s) {
	int len = strlen(s);

	if (len < 2 || len > 3) {
		return 0;
	}

	if (s[0] != 'o' && s[0] != 'O') {
		return 0;
	}


	if (s[1] == 'n' || s[1]=='N') {
		if (len == 3) {
			return 0;
		}
	} else if (s[1] == 'f' || s[1]=='F') {
		if (len == 2 || (s[2] != 'f' && s[2] != 'F')) {
			return 0;
		}
	} else {
		//second letter incorrect
		return 0;
	}


	return 1;
}
