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
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "usb.h"
#include "mx.h"

#define HELP(x) help(x,argv[0])
static void help(int status, char *program_name);

int main(int argc, char **argv){
	int err,opt;
	char *command;
	MXCommand action = NULL;
	const char *short_opt = "hv";
	struct option long_opt[] = {
		{"help", 	no_argument, NULL, 'h'},
		{"version",	no_argument, NULL, 'v'},
		{NULL, 		0,			 NULL,	0}
	};

	while ((opt = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1 ) {
		switch (opt) {
			case 'v': printf("mx3100drv v%s\n", VERSION); exit(0);
			case 'h': HELP(0);
			case ':': case '?': default: HELP(-2);
		}
	}


	if (argv[optind] == NULL){
		fprintf(stderr, "A command is required. None provided\n");
		HELP(-2);
	}
	command = argv[optind];

	if (strcmp(command,"snap") == 0) {
		action = angle_snap;
	} else if (strcmp(command,"angle") == 0) {
		action = angle_correct;
	} else if (strcmp(command,"led") == 0) {
		action = led_mode;
	} else if (strcmp(command,"brightness")==0){
		action = led_brightness;
	} else if (strcmp(command,"speed") == 0) {
		action = led_speed;
	} else if (strcmp(command,"sensitivity") == 0) {
		action = sensitivity;
	} else if (strcmp(command,"dpi") == 0) {
		if ( argc-optind-1 == 0 ) {
			fprintf(stderr, "dpi commands require a profile number\n");
			HELP(-2);
		}
		if ( argc-optind-1 == 1 || strcmp(argv[optind+2],"on") == 0 || strcmp(argv[optind+2],"off") == 0) {
			action = dpi_enable;
		} else if (strcmp(argv[optind+2],"color") == 0){
			action = dpi_color;
		} else if (strcmp(argv[optind+2],"value") == 0){
			action = dpi_value;
		} else {
			fprintf(stderr, "Invalid dpi command\n");
			HELP(-2);
		}
	/* a bit broken */
	/*} else if (strcmp(command,"reset") == 0) {
		action = factory_reset;*/
	} else if (strcmp(command,"dump") == 0) {
		action = save_info;
	} else {
		fprintf(stderr, "%s is not a valid command.\n", command);
		HELP(-2);
	}

	if (action == NULL) {
		fprintf(stderr, "Command not found or implemented yet.\n");
		return -1;
	}


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

	err = action(argc-optind-1, argv+(optind+1));
	finish_usb();
	return err;
}



static void help(int status, char *pgname) {
	const char *usage = "Usage: %s [OPTIONS] COMMAND [ARGUMENTS]\n"
			 "\tOptions:\n"
			 "\t\t-h,--help\tPrint this usage info and exit\n"
			 "\t\t-p,--profile=PROFILE\tSelect a particular profile\n"
			 "\t\t-v,--verbose\tIncrease debug output\n"
			 "\t\t-V,--version\tPrint program version info and exit\n"
			 "\tCommands:\n"
			 "\t\treset\tResets mouse to factory settings (careful!)\n"
			 "\t\tdump FILE\tSave current mouse configuration to FILE\n"
			 "\t\tload FILE\tRestore mouse configuration to settings in FILE\n"
			 "See man page for additional commands and settings options\n";
	printf(usage, pgname);
	exit(status);
}
