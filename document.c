#include <string.h>

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

