#define _DEFAULT_SOURCE

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define EXTRA_SIZE 256
#define BLOCK_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];

  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);

  if (len < 0) {
    handle_error("snprintf");
  }

  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  char *heap_start = sbrk(EXTRA_SIZE);

  if (heap_start == (void *)-1) {
    handle_error("sbrk");
  }

  struct header *first = (struct header *)heap_start;
  struct header *second = (struct header *)(heap_start + BLOCK_SIZE);

  first->size = BLOCK_SIZE;
  first->next = NULL;

  second->size = BLOCK_SIZE;
  second->next = first;

  char *first_data = (char *)(first + 1);
  char *second_data = (char *)(second + 1);

  size_t data_size = BLOCK_SIZE - sizeof(struct header);

  memset(first_data, 0, data_size);
  memset(second_data, 1, data_size);

  print_out("first block:       %p\n", &first, sizeof(first));
  print_out("second block:      %p\n", &second, sizeof(second));

  print_out("first block size:  %lu\n", &first->size, sizeof(first->size));
  print_out("first block next:  %p\n", &first->next, sizeof(first->next));

  print_out("second block size: %lu\n", &second->size, sizeof(second->size));
  print_out("second block next: %p\n", &second->next, sizeof(second->next));

  for (size_t i = 0; i < data_size; i++) {
    print_out("%lu\n", (uint64_t *)&(uint64_t){(uint64_t)first_data[i]}, sizeof(uint64_t));
  }

  for (size_t i = 0; i < data_size; i++) {
    print_out("%lu\n", (uint64_t *)&(uint64_t){(uint64_t)second_data[i]}, sizeof(uint64_t));
  }

  return 0;
}
