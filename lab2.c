#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  char path[256];

  while (1) {
    printf("Enter programs to run.\n> ");

    if (fgets(path, sizeof(path), stdin) == NULL) {
      break;
    }

    path[strcspn(path, "\n")] = '\0';

    pid_t pid = fork();

    if (pid < 0) {
      perror("fork failed");
    } else if (pid == 0) {
      execl(path, path, NULL);
      printf("Exec failure\n");
      exit(1);
    } else {
      waitpid(pid, NULL, 0);
    }
  }

  return 0;
}
