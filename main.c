#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "helpers.c"

#define MAX_LINE_LENGTH 256
#define MAX_OPS 1024

int main(int argc, char *argv[]) {
  bool stacktrace = false;  
  for (int i = 0; i < argc; i++) {
	if (strcmp(argv[i], "--trace") == 0) {
	  stacktrace = true;
	}
  }

  // Store all lines that are not comments or empty line in
  // a temporary array that is used for parsing etc.

  FILE *in_file;
  char *erdfilename = "Errandfile";
  
  in_file = fopen(erdfilename, "r");
  if (in_file == NULL) {
	fprintf(stderr, "Error: could not open file %s with error %s\n", erdfilename, strerror(errno));
	return 1;
  }
  
  char **lines = NULL;
  size_t num_lines = 0;
  char line[MAX_LINE_LENGTH];
  while(fgets(line, MAX_LINE_LENGTH, in_file) != NULL) {
	strtrim(line);
	if (strncmp(line, "#", 1) != 0 && strlen(line) > 0) {
	  char *new_line = malloc(MAX_LINE_LENGTH);
	  if (new_line == NULL) {
		fprintf(stderr, "Error: memory allocation failed.\n");
		return 1;
	  }

	  strcpy(new_line, line);

	  char **temp = realloc(lines, (num_lines + 1) * sizeof(char *));
	  if (temp == NULL) {
		fprintf(stderr, "Error: memory allocation failed.\n");
		free(new_line);
		return 1;
	  }
	  lines = temp;

	  lines[num_lines] = new_line;
	  num_lines++;
	}
  }

  fclose(in_file);

  // Display stacktrace if `--trace` is in arguments and exit.

  if (stacktrace) {
	printf("Stack trace:\n");
	for (size_t i=0; i<num_lines; ++i) {
	  printf("  %lu: %s\n", i+1, lines[i]);
	}
	return 1;
  }

  // Display all available targets in provided Errandfile.

  if (argc == 1) {
	printf("Available targets:\n");
	for (size_t i=0; i<num_lines; ++i) {
	  if (strncmp(lines[i], "@task", 5) == 0) {
		char op[MAX_LINE_LENGTH];
		char name[MAX_LINE_LENGTH];
		char description[MAX_LINE_LENGTH];
		
		parse_taskdef(lines[i], op, name, description, MAX_LINE_LENGTH);
		remove_quotes(description);

		printf("  %-15s %s\n", name, description);		
	  }
	}
	return 1;
  }
  
  free(lines);
  return 0;
}
