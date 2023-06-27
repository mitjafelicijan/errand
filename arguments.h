// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdbool.h>
#include <unistd.h>

typedef struct {
  char *erdfilename;
  bool stacktrace;
  bool help;
  bool version;
  char **tasks;
  size_t num_tasks;
} Arguments;

Arguments argparse(int argc, char *argv[]);

#endif
