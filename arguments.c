// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#include "arguments.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"

Arguments argparse(int argc, char *argv[]) {
	Arguments args = {0};
	args.debug = false;
	args.help = false;
	args.erdfilename = "Errandfile";

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-f") == 0) {
			if (i + 1 < argc) {
				char *tmp_erdfilename = argv[i + 1];
				if (access(tmp_erdfilename, F_OK) == -1) {
					fprintf(stderr, "Error: file %s does not exist\n", tmp_erdfilename);
					exit(1);
				}

				args.erdfilename = tmp_erdfilename;
				i++;
			} else {
				fprintf(stderr, "Error: file was not provided\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
			args.debug = true;
		} else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
			args.help = true;
		} else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
			args.version = true;
		} else {
			char *task = argv[i];
			char *new_task = malloc(MAX_LINE_LENGTH);
			if (new_task == NULL) {
				fprintf(stderr, "Error: memory allocation failed.\n");
				exit(1);
			}

			strcpy(new_task, task);

			char **temp = realloc(args.tasks, (args.num_tasks + 1) * sizeof(char *));
			if (temp == NULL) {
				fprintf(stderr, "Error: memory allocation failed.\n");
				free(new_task);
				exit(1);
			}
			args.tasks = temp;
			args.tasks[args.num_tasks] = new_task;
			args.num_tasks++;
		}
	}

	return args;
}
