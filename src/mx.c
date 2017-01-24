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
#include <stdlib.h> /* free */
#include <string.h> /* memcpy */
#include "usb.h"
#include "mx.h"

int read_section(unsigned char addr, unsigned char *buf) {
	unsigned char cmd[CMD_MSG_LEN] = {ADDR_READ,0x00,0x00,0x00,0x00,0x00,0x00,addr};
	unsigned char rsp[CMD_MSG_LEN];
	int err;

	if ( addr == CONFIGS_ADDR || addr == BUTTONS_ADDR ) {
		cmd[0] |= SETTINGS_ADDR_MAX-addr + SETTINGS_ADDR_PARITY;
	} else {
		cmd[0] |= MACRO_MEM_FLAG;
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

int send_startup_cmds(void) {
	int err;

	unsigned char start1[CMD_MSG_LEN] = {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0xfd};
	unsigned char start2[CMD_MSG_LEN] = {0x03,0x00,0x02,0x00,0x00,0x00,0x00,0xfa}; /* @todo poll rate */
	err = send_ctl(start1);
	err = send_ctl(start2);
	return err;
}
