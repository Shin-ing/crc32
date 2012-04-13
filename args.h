/*	args.h - argument handling
	Copyright (C) 2012 Connor Olding

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ARGS_H_
#define ARGS_H_

#include <stdio.h>
#include <string.h>

/* interface */

extern char args_info[];
extern char args_usage[];
extern char args_help_suffix[];

extern const int args_count;
extern char* args_args[];
extern void (*args_functions[])();

void args_handle(int argc, char* argv[]);
char* args_poll();
void args_print_help();

/* implemenation */

static const int args_columns = 3;

static char* args_program_name = NULL;

static int args_argc = 0;
static char** args_argv = NULL;

static int args_current = 0;
static int args_previous = 0;

static void args_print_info()
{
	printf(args_info);
}

static void args_print_usage()
{
	printf(args_usage, args_program_name);
}

static void args_print_args()
{
	int i;
	for (i = 0; i < args_count * args_columns; i += args_columns) {
		printf("  %s, %s %s\n",
			args_args[i], args_args[i + 1], args_args[i + 2]);
	}
	printf(args_help_suffix);
}

void args_print_help()
{
	args_print_info();
	puts("");
	args_print_args();
	exit(0);
}

static int args_get_index(char* name)
{
	int i;
	for (i = 0; i < args_count * args_columns; i++) {
		if (i % args_columns == args_columns - 1)
			continue; /* skip checking the description column */
		if (!strcmp(args_args[i], name)) {
			return i / args_columns;
		}
	}
	return -1;
}

char* args_poll()
{
	if (args_current >= args_argc) {
		if (args_current - 1 == args_previous)
			fprintf(stderr, "%s requires an argument.\n",
				args_argv[args_previous]);
		else
			fprintf(stderr, "%s requires another argument.\n",
				args_argv[args_previous]);
		args_print_usage();
		exit(1);
	}
	args_current++;
	return args_argv[args_current - 1];
}

void args_handle(int argc, char** argv)
{
	args_argc = argc;
	args_argv = argv;
	
	args_current = 0;
	args_program_name = args_poll();
	
	while (args_current < args_argc) {
		char* name;
		int index;
		
		args_previous = args_current;
		name = args_poll();
		index = args_get_index(name);
		if (index < 0) {
			fprintf(stderr, "Unknown option: %s\n", name);
			args_print_usage();
			exit(1);
		}
		args_functions[index]();
	}
}

#endif
