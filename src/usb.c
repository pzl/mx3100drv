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

#include <stdio.h>
#include <string.h>
#include <hidapi/hidapi.h>
#include "usb.h"

hid_device *device;

int initialize_usb(void) {
	if ( hid_init() != 0){
		return ERR_NO_HID;
	}
	return 0;
}

int find_device(void) {
	struct hid_device_info *list,
						   *current,
						   *matches[MX_MAX_MICE];
	int result;
	unsigned int i, choice, n_matches = 0;
	char path[MX_PATH_BUF];


	if ((list = hid_enumerate(MX_VENDOR_ID, MX_PRODUCT_ID)) == NULL){
		return ERR_NO_DEVICE;
	}
	current = list;
	while (current) {
		if ( current->interface_number == MX_CONTROL_INTERFACE || // linux + windows, normal
			( current->interface_number== -1 && current->path[strlen(current->path)-1] == MX_CONTROL_INTERFACE + '0' )) {
			// OSX only gives -1 as interface_number, and makes it really difficult to determine which interface
			// On recent releases (AFTER, NOT INCLUDING 0.8.0) of hidapi, the last character of path is interface
			matches[n_matches++] = current;
		}
		current = current->next;
	}

	if (n_matches > 1) {
		printf("%u mice were found, use which one?\n", n_matches);
		for (i=0; i<n_matches; i++){
			printf("\t%u: %s\n", i+1, matches[i]->path);
		}
		result = scanf("%u",&choice);
		if ( result == EOF || result == 0 ) {
			fprintf(stderr, "No choice entered, exiting.\n");
			hid_free_enumeration(list);
			return ERR_GET_DEVICE;
		}
		choice--;
	} else {
		choice = 0;
	}

	strncpy(path, matches[choice]->path, MX_PATH_BUF);
	path[sizeof(path)-1] = '\0';

	hid_free_enumeration(list);

	device = hid_open_path(path);
	if (!device) {
		fprintf(stderr, "Error opening mouse: %ls\n", hid_error(device));
		return ERR_GET_DEVICE;
	}
	return 0;
}

/*
	Sends commands to the CONTROL endpoint
	buf: unsigned char[8] - command to send

	returns: 0 on success
			negative for err
*/
int send_ctl(unsigned char *buf) {
	int err;
	unsigned char cmd[9];

	memcpy( &cmd[1], buf, 8 );
	cmd[0] = 0;

	/* Make sure we have a valid device handle */
	if (device == NULL){
		fprintf(stderr, "No device open for controlling\n");
		return ERR_NO_DEVICE;
	}
	err = hid_send_feature_report(device, cmd, 9);

	if (err < 0){
		fprintf(stderr, "control error: %ls\n", hid_error(device));
		return -2;
	}
	return 0;
}

int read_ctl(unsigned char *buf){
	int err;
	unsigned char response[9] = {0};

	/* Make sure we have a valid device handle */
	if (device == NULL){
		fprintf(stderr, "No device open for controlling\n");
		return ERR_NO_DEVICE;
	}
	err = hid_get_feature_report(device, response, 9);

	if (err < 0){
		fprintf(stderr, "readback error: %ls\n", hid_error(device));
		return ERR_CMD;
	}
	if (err < 9){
		fprintf(stderr, "Possible data underflow when receiving ACK from mouse. Got %d bytes back\n", err);
		return ERR_CMD;
	}

	memcpy( buf, &response[1], 8 );

	return 0;
}

/*
	Writes data to the device
	buf: unsigned char[64]
*/
int send_data( unsigned char *buf ){
	int err;

	if (device == NULL){
		fprintf(stderr, "No device open for writing\n");
		return ERR_NO_DEVICE;
	}
	err = hid_write(device,buf,64);

	if (err<0){
		fprintf(stderr, "Data write error: %ls\n", hid_error(device));
		return -2;
	}
	return 0;
}

int read_data( unsigned char *buf ){
	int err;

	if (device == NULL){
		fprintf(stderr, "No device open for reading\n");
		return ERR_NO_DEVICE;
	}
	err = hid_read_timeout(device, buf, 64, 2000);

	if (err<0){
		fprintf(stderr, "Data read error: %ls\n", hid_error(device));
		return -2;
	}
	if (err < 64) {
		fprintf(stderr, "Possible data underflow when reading from mouse. Got %d bytes back\n", err);
		return -2;
	}
	return 0;
}


void finish_usb(void) {
	if (device){
		hid_close(device);
	}
	hid_exit();
}
