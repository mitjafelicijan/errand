#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "helpers.c"
#include "document.c"
#include "shell.c"

#define VERSION "1.1"
#define MAX_LINE_LENGTH 1024
#define MAX_OPS 16*1024

typedef struct {
  char *erdfilename;
  bool stacktrace;
  bool help;
  bool version;
  char **tasks;
  size_t num_tasks;
} Definition;

typedef struct {
  char **lines;
  size_t num_lines;
} Lines;

Definition argparse(int argc, char *argv[]) {
  Definition d = {0};
  d.stacktrace = false;
  d.help = false;
  d.erdfilename = "Errandfile";
  
  for (int i=1; i<argc; ++i) {
	if (strcmp(argv[i], "-f") == 0) {
	  if (i+1 < argc) {
		char *tmp_erdfilename = argv[i+1];
		if (access(tmp_erdfilename, F_OK) == -1) {
		  fprintf(stderr, "Error: file %s does not exist\n", tmp_erdfilename);
		  exit(1);
		}

		d.erdfilename = tmp_erdfilename;
		i++;
	  } else {
		fprintf(stderr, "Error: file was not provided\n");
		exit(1);
	  }
	}
	else if (strcmp(argv[i], "--trace") == 0) {
	  d.stacktrace = true;
	}
	else if (strcmp(argv[i], "--help") == 0) {
	  d.help = true;
	}
	else if (strcmp(argv[i], "--version") == 0) {
	  d.version = true;
	}
	else {
	  char *task = argv[i];
	  char *new_task = malloc(MAX_LINE_LENGTH);
	  if (new_task == NULL) {
		fprintf(stderr, "Error: memory allocation failed.\n");
		exit(1);
	  }

	  strcpy(new_task, task);

	  char **temp = realloc(d.tasks, (d.num_tasks + 1) * sizeof(char *));
	  if (temp == NULL) {
		fprintf(stderr, "Error: memory allocation failed.\n");
		free(new_task);
		exit(1);
	  }
	  d.tasks = temp;
	  d.tasks[d.num_tasks] = new_task;
	  d.num_tasks++;
	}
  }

  return d;
}

int main(int argc, char *argv[]) {
  Definition d = argparse(argc, argv);

  if (d.version) {
	printf("Errand %s\n", VERSION);
	return 0;
  }
  
  if (d.help) {
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

  FILE *in_file;
  in_file = fopen(d.erdfilename, "r");
  if (in_file == NULL) {
	fprintf(stderr, "Error: could not open file %s with error %s\n", d.erdfilename, strerror(errno));
	return 1;
  }

  Lines l = {0};
  char line[MAX_LINE_LENGTH];
  while(fgets(line, MAX_LINE_LENGTH, in_file) != NULL) {
	strtrim(line);
	if (strncmp(line, "#", 1) != 0 && strlen(line) > 0) {
	  char *new_line = malloc(MAX_LINE_LENGTH);
	  if (new_line == NULL) {
		fprintf(stderr, "Error: memory allocation failed.\n");
		exit(1);
	  }

	  strcpy(new_line, line);

	  char **temp = realloc(l.lines, (l.num_lines + 1) * sizeof(char *));
	  if (temp == NULL) {
		fprintf(stderr, "Error: memory allocation failed.\n");
		free(new_line);
		return 1;
	  }
	  l.lines = temp;

	  l.lines[l.num_lines] = new_line;
	  l.num_lines++;
	}
  }

  fclose(in_file);

  // Display stacktrace if `--trace` is in arguments and exit.

  if (d.stacktrace) {
	printf("Stack trace:\n");
	for (size_t i=0; i<l.num_lines; ++i) {
	  printf("  %lu: %s\n", i+1, l.lines[i]);
	}
	exit(1);
  }

  // Display all available targets in provided Errandfile
  // if no targets provided with arguments.

  if (d.num_tasks == 0) {
	printf("Available targets:\n");
	for (size_t i=0; i<l.num_lines; ++i) {
	  if (strncmp(l.lines[i], "@task", 5) == 0) {
		char op[MAX_LINE_LENGTH];
		char name[MAX_LINE_LENGTH];
		char description[MAX_LINE_LENGTH];
		
		parse_taskdef(l.lines[i], op, name, description, MAX_LINE_LENGTH);
		remove_quotes(description);

		printf("  %-14s %s\n", name, description);		
	  }
	}
	return 1;
  }

  // TODO: Execute provided targets via arguments.

  printf(">> Errandfile: %s\n", d.erdfilename);
  printf(">> Stacktrace: %d\n", d.stacktrace);
  printf(">> Tasks:\n");
  
  for (size_t i=0; i<d.num_tasks; ++i) {
	printf(" - %lu: %s\n", i, d.tasks[i]);
  }
  
  return 0;
}
