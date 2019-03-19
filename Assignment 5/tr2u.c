#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv) {
  int i, j;
  char buf[1];
  char *from;
  char *to;

  if (argc < 3) {
    fprintf(stderr, "%s\n", "missing operands");
    exit(1);
  }

  if (argc > 3) {
    fprintf(stderr, "%s\n", "extra operands");
    exit(1);
  }

  from = argv[1];
  to = argv[2];

  if (strlen(from) != strlen(to)) {
    fprintf(stderr, "%s\n", "two operands must have same length");
    exit(1);
  }

  for (i = 0; i < strlen(from); i++) {
    for (j = i + 1; j < strlen(from); j++) {
      if (from[i] == from[j]) {
        fprintf(stderr, "%s\n", "first operand may not contain any duplicate byte");
        exit(1);
      }
    }
  }

  ssize_t flag;

  flag = read(0, buf, 1);
  if (flag == -1) {
    fprintf(stderr, "%s\n", errno);
    exit(1);
  }

  while (flag != -1 && flag != 0) {

    for (i = 0; i < strlen(from); i++) {
      if (buf[0] == from[i]) {
        buf[0] = to[i];
        break;
      }
    }

    flag = write(1, buf, 1);
    if (flag == -1) {
      fprintf(stderr, "%s\n", errno);
      exit(1);
    }

    flag = read(0, buf, 1);
    if (flag == -1) {
      fprintf(stderr, "%s\n", errno);
      exit(1);
    }
  }

  return 0;
}