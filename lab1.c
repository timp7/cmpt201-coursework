#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *buffer = NULL;
  size_t buffer_size = 0;

  while (1) {
    printf("Please enter some text: ");
    ssize_t chars_read = getline(&buffer, &buffer_size, stdin);

    if (chars_read == -1) {
      perror("getline");
      free(buffer);
      exit(1);
    }
    if (chars_read == 1 && buffer[0] == '\n') {
      break;
    }

    if (buffer[chars_read - 1] == '\n') {
      buffer[chars_read - 1] = '\0';
    }

    printf("Tokens:\n");

    char *saveptr;
    char *token = strtok_r(buffer, " ", &saveptr);

    while (token != NULL) {
      printf(" %s\n", token);
      token = strtok_r(NULL, " ", &saveptr);
    }
  }

  free(buffer);
  return 0;
}
