#include <string.h>
#include <ctype.h>

void strtrim(char *str) {
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

void remove_quotes(char* str) {
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