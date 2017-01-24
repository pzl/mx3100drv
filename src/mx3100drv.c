/*
MX3100DRV - portable driver for the MX-3100 gaming mouse
Copyright (C) 2015  Dan Panzarella

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
#include "usb.h"
#include "mx.h"

int main(int argc, char **argv){
	int err;

	unsigned char data[DATA_LINE_LEN*DATA_LINES];
	int i;


	err = initialize_usb();
	if (err<0){
		finish_usb();
		return err;
	}

	err = find_device();
	if (err<0){
		finish_usb();
		fprintf(stderr, "Error: Mouse not found, is it plugged in?\n");
		return err;
	}


	err = send_startup_cmds();
	if (err != 0){
		fprintf(stderr, "error sending startup commands\n");
		finish_usb();
		return err;
	}

	err = read_section(0x72,data);

	for (i=0; i<DATA_LINES*DATA_LINE_LEN; i++){
		printf("0x%02X, ", data[i]);
	}
	printf("\n");



	finish_usb();
	return 0;
}
