// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constants.h"
#include "arguments.h"
#include "document.h"
#include "shell.h"

int main(int argc, char *argv[]) {
  Arguments args = argparse(argc, argv);
  
  if (args.version) {
    time_t ctime = time(NULL);
    struct tm* local = localtime(&ctime);
    int year = local->tm_year + 1900;

	printf("Errand %s - Simple task runner\n", VERSION);
	printf("Copyright (C) 2022-%d Mitja Felicijan\n", year);
	printf("License BSD 2-Clause: <https://opensource.org/licenses/BSD-2-Clause>\n");
	printf("This is free software: you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
	return 0;
  }
  
  if (args.help) {
	printf("Usage: erd [options] [task] ...\n\n");
	printf("Options:\n");
	printf("  %-15s %s\n", "-f [FILE]", "Use a different file (default Errandfile).");
	printf("  %-15s %s\n", "-h --help", "Print this message and exit.");
	printf("  %-15s %s\n", "-d --debug", "Show debug of the parsed file.");	
	printf("  %-15s %s\n", "-v --version", "Print the version number and exit.");	
	return 0;
  }

  // Store all lines that are not comments or empty line in
  // a temporary array that is used for parsing etc.
  Lines l = read_erdfile(args.erdfilename);
  
  // Display debug information if `--debug` is in arguments and exit.
  if (args.debug) {
	printf("Contents of parsed file:\n");
	for (size_t i=0; i<l.num_lines; ++i) {
	  printf("  %lu: %s\n", i+1, l.lines[i]);
	}
	return 0;
  }

  // Display all available targets in provided Errandfile
  // if no targets provided with arguments.
  if (args.num_tasks == 0) {
	printf("Available targets:\n");
	for (size_t i=0; i<l.num_lines; ++i) {
	  if (strncmp(l.lines[i], "@task", 5) == 0) {
		char op[MAX_LINE_LENGTH];
		char name[MAX_LINE_LENGTH];
		char description[MAX_LINE_LENGTH];
		
		parse_taskdef(l.lines[i], op, name, description, MAX_LINE_LENGTH);
		rm_quotes(description);

		printf("  %-14s %s\n", name, description);		
	  }
	}
	return 0;
  }

  // TODO: Execute provided targets via arguments.
  printf(">> Errandfile: %s\n", args.erdfilename);
  printf(">> Debug: %d\n", args.debug);
  printf(">> Tasks:\n");
  
  for (size_t i=0; i<args.num_tasks; ++i) {
	printf(" - %lu: %s\n", i, args.tasks[i]);
  }
  
  return 0;
}
