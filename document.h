// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <stdio.h>

typedef struct {
	char **lines;
	size_t num_lines;
} Lines;

void str_trim(char *str);
void rm_quotes(char* str);
void parse_taskdef(const char *text, char *op, char *name, char *description, size_t max_length);
Lines read_erdfile(char *erdfilename);

#endif
