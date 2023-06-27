// SPDX-License-Identifier: BSD-2-Clause
// Copyright (c) 2023, Mitja Felicijan <m@mitjafelicijan.com>

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "constants.h"

void run_task(char *shell, char *script) {
  char * erdfifo = FIFO_FILE;
  mkfifo(erdfifo, 0666);
  FILE *fp;
  char command[4*1024];
  char output[1024];

  sprintf(command, "%s > %s", shell, erdfifo);
  fp = popen(command, "w");

  // fprintf(fp, "ls -l;ls -lha;sleep 1;docker-compose -f test-data/stack.yml up;exit;\n");
  fprintf(fp, "%s", script);
  fflush(fp);
  FILE* fpr = fopen(erdfifo, "r");

  while (fgets(output, 1024, fpr) != NULL) {
	printf("%s", output);
  }

  pclose(fp);
  fclose(fpr);
  unlink(erdfifo);
}
