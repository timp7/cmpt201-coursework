#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

void add_to_history(char *history[], int *count, char *line) {
  if (*count < HISTORY_SIZE) {
    history[*count] = line;
    (*count)++;
  } else {
    free(history[0]);

    for (int i = 1; i < HISTORY_SIZE; i++) {
      history[i - 1] = history[i];
    }

    history[HISTORY_SIZE - 1] = line;
  }
}

void print_history(char *history[], int count) {
  for (int i = 0; i < count; i++) {
    printf("%s", history[i]);
  }
}

int main(void) {
  char *history[HISTORY_SIZE] = {NULL};
  int count = 0;

  char *line = NULL;
  size_t size = 0;

  while (1) {
    printf("Enter input: ");

    ssize_t chars_read = getline(&line, &size, stdin);

    if (chars_read == -1) {
      break;
    }

    char *saved_line = malloc(chars_read + 1);

    if (saved_line == NULL) {
      printf("Memory allocation failed.\n");
      free(line);
      return 1;
    }

    strcpy(saved_line, line);

    add_to_history(history, &count, saved_line);

    if (strcmp(line, "print\n") == 0) {
      print_history(history, count);
    }
  }

  free(line);

  for (int i = 0; i < count; i++) {
    free(history[i]);
  }

  return 0;
}
