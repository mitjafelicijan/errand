// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#include "document.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

void str_trim(char *str) {
	size_t len = strlen(str);
	size_t start = 0;
	size_t end = len - 1;

	while (start < len && isspace((unsigned char)str[start])) start++;
	while (end > start && isspace((unsigned char)str[end])) end--;

	size_t shift = end - start + 1;
	if (start > 0) {
		memmove(str, str + start, shift);
	}

	str[shift] = '\0';
}

void rm_quotes(char *str) {
	int length = strlen(str);
	if (length >= 2 && str[0] == '"' && str[length - 1] == '"') {
		memmove(str, str + 1, length - 2);
		str[length - 2] = '\0';
	}
}

void parse_taskdef(const char *text, char *op, char *name, char *description, size_t max_length) {
	char copy[max_length];
	strncpy(copy, text, max_length);

	char *token = strtok(copy, " ");
	if (token != NULL) {
		strncpy(op, token, max_length);
		token = strtok(NULL, " ");

		if (token != NULL) {
			strncpy(name, token, max_length);
			token = strtok(NULL, "");

			if (token != NULL) {
				strncpy(description, token, max_length);
			}
		}
	}
}

Lines read_erdfile(char *erdfilename) {
	FILE *in_file;
	in_file = fopen(erdfilename, "r");
	if (in_file == NULL) {
		fprintf(stderr, "Error: could not open file %s with error %s\n", erdfilename, strerror(errno));
		exit(1);
	}

	Lines l = {0};
	char line[MAX_LINE_LENGTH];
	while (fgets(line, MAX_LINE_LENGTH, in_file) != NULL) {
		str_trim(line);
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
				exit(1);
			}
			l.lines = temp;

			l.lines[l.num_lines] = new_line;
			l.num_lines++;
		}
	}

	fclose(in_file);

	return l;
}
