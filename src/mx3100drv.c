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
	int err,
		opt,
		verbose=0;
	char *command;
	MXCommand action = NULL;
	const char *short_opt = "hvV";
	struct option long_opt[] = {
		{"help", 	no_argument, NULL, 'h'},
		{"verbose", no_argument, NULL, 'v'},
		{"version",	no_argument, NULL, 'V'},
		{NULL, 		0,			 NULL,	0}
	};

	while ((opt = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1 ) {
		switch (opt) {
			case 'v': verbose = 1; break;
			case 'V': printf("mx3100drv v%s\n", VERSION); exit(0);
			case 'h': HELP(0);
			case ':': case '?': HELP(-2);
			default:
				fprintf(stderr, "%s: invalid option -- %c\n", argv[0], opt);
				HELP(-2);
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

	err = action(argc-optind-1, argv+(optind+1), verbose);
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
