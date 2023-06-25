#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_LINE_LENGTH 200

int main() {
  FILE *in_file;
  char *erdfilename = "Errandfile";

  in_file = fopen(erdfilename, "r");
  if (in_file == NULL) {
	fprintf(stderr, "Error: could not open file %s with error %s\n", erdfilename, strerror(errno));
	return 1;
  }

  char line[MAX_LINE_LENGTH];
  while(fgets(line, MAX_LINE_LENGTH, in_file) != NULL) {
	printf("%s", line);
  }
  
}
