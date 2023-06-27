// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "arguments.h"
#include "document.h"
#include "shell.h"

int main(int argc, char *argv[]) {
  Arguments args = argparse(argc, argv);
  
  if (args.version) {
	printf("Errand %s\n", VERSION);
	return 0;
  }
  
  if (args.help) {
	printf("Usage: erd [options] [task] ...\n\n");
	printf("Options:\n");
	printf(" %-10s %s\n", "-f [FILE]", "Use a different file (default Errandfile).");
	printf(" %-10s %s\n", "--help", "Print this message and exit.");
	printf(" %-10s %s\n", "--trace", "Show trace of the parsed file.");	
	printf(" %-10s %s\n", "--version", "Print the version number and exit.");	
	return 0;
  }

  // Store all lines that are not comments or empty line in
  // a temporary array that is used for parsing etc.
  Lines l = read_erdfile(args.erdfilename);
  
  // Display stacktrace if `--trace` is in arguments and exit.
  if (args.stacktrace) {
	printf("Stack trace:\n");
	for (size_t i=0; i<l.num_lines; ++i) {
	  printf("  %lu: %s\n", i+1, l.lines[i]);
	}
	exit(1);
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
	return 1;
  }

  // TODO: Execute provided targets via arguments.
  printf(">> Errandfile: %s\n", args.erdfilename);
  printf(">> Stacktrace: %d\n", args.stacktrace);
  printf(">> Tasks:\n");
  
  for (size_t i=0; i<args.num_tasks; ++i) {
	printf(" - %lu: %s\n", i, args.tasks[i]);
  }
  
  return 0;
}
