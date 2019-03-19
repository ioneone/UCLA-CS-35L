#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int i, j;
  char c;
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

  c = getchar();
  if (ferror(stdin)) {
    fprintf(stderr, "%s\n", "stdin error");
    exit(1);
  }

  while (!feof(stdin)) {

    for (i = 0; i < strlen(from); i++) {
      if (c == from[i]) {
        c = to[i];
        break;
      }
    }

    putchar(c);
    if (ferror(stdout)) {
      fprintf(stderr, "%s\n", "stdout error");
      exit(1);
    }

    c = getchar();
    if (ferror(stdin)) {
      fprintf(stderr, "%s\n", "stdin error");
      exit(1);
    }
  }

  return 0;
}