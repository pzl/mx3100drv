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

#ifndef MX_USB_H
#define MX_USB_H

#define MX_VENDOR_ID 0x04d9
#define MX_PRODUCT_ID 0xa11b
#define MX_CONTROL_INTERFACE 2

#define MX_PATH_BUF 256

#define MX_MAX_MICE 10

#define ERR_NO_HID -1
#define ERR_KERNEL_DRIVER -2
#define ERR_GET_DEVICE -3
#define ERR_NO_DEVICE -4
#define ERR_CMD -5

int initialize_usb(void);
void finish_usb(void);

int find_device(void);

int send_ctl(unsigned char *buf);
int read_ctl(unsigned char *buf);
int send_data(unsigned char *buf);
int read_data(unsigned char *buf);

#endif
