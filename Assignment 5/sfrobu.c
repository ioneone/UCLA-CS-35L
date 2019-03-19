#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

int frobcmp(const void *c1, const void *c2) {
  const char *word1 = *(const char**)c1;
  const char *word2 = *(const char**)c2;
  int i = 0;

  while (word1[i] != ' ' && word2[i] != ' ') {
    if (word1[i] == word2[i]) {
      i++;
    }
    else {
      return ((word1[i] ^ 42) > (word2[i] ^ 42)) ? 1 : -1;
    }
  }

  if (word1[i] == ' ' && word2[i] == ' ') {
    return 0;
  }

  if (word1[i] == ' ' && word2[i] != ' ') {
    return -1;
  }

  return 1;
}

int frobcmpF(const void *c1, const void *c2) {
  const char *word1 = *(const char**)c1;
  const char *word2 = *(const char**)c2;
  int i = 0;

  while (word1[i] != ' ' && word2[i] != ' ') {
    if (word1[i] == word2[i]) {
      i++;
    }
    else {
      return (toupper((unsigned char)(word1[i] ^ 42)) > toupper((unsigned char)(word2[i] ^ 42))) ? 1 : -1;
    }
  }

  if (word1[i] == ' ' && word2[i] == ' ') {
    return 0;
  }

  if (word1[i] == ' ' && word2[i] != ' ') {
    return -1;
  }

  return 1;
}


void freeData(char **data, int row) {
  if (data == NULL) return;

  // free memory allocation
  int i;
  for (i = 0; i < row; i++) {
    free(data[i]);
  }
  free(data);
}

void freeInput(char **data, char *input) {
  if (input != NULL) {
    free(input);
  }
  if (data != NULL) {
    free(data);
  }
}

int isNULLPtr(void *ptr) {
  if (ptr == NULL) {
    fprintf(stderr, "Memory Allocation Error\n");
    return 1;
  }

  return 0;
}

int printData(char **data, int row) {
  int i;
  for (i = 0; i < row; i++) {
    int j = 0;
    while (data[i][j] != ' ') {
      int flag = write(1, &data[i][j++], 1);
      if (flag == -1) {
        fprintf(stderr, "%s\n", "write error");
        return -1;
      }
    }
    
    int flag = write(1, " ", 1);
    if (flag == -1) {
      fprintf(stderr, "%s\n", "write error");
      return -1;
    }
  }

  return 0;
  
}

int main(int argc, char **argv) {
  char **data = NULL;
  char *input = NULL;
  int i;
  int row;
  int ptr;
  int isFound;
  int f_option = 0;

  // statbuf contains information about input file
  struct stat statbuf;

  // ============
  // check option
  // ============
  int opt;
  while ((opt = getopt(argc, argv, "f")) != -1) {
    switch (opt) {
      case 'f':
        f_option = 1;
        break;
      case '?':
        fprintf(stderr, "%s -%c\n", "Unknown option", optopt);
        exit(1);
        break;
      default:
        break;
    }
  }

  if (fstat(0, &statbuf) == -1) {
    fprintf(stderr, "%s\n", "fstat error");
    exit(1);
  }

  // =============================
  // if input is a file
  // allocate enough memory
  // =============================
  if (S_ISREG(statbuf.st_mode)) {

    // empty file
    if (statbuf.st_size == 0) return 0;   

    input = (char*)malloc(statbuf.st_size*sizeof(char));
    if (isNULLPtr(input)) {
      exit(1);
    }
    
    if (read(0, input, statbuf.st_size) == -1) {
      fprintf(stderr, "%s\n", "read error");
      free(input);
      exit(1);
    }
    
    // check number of spaces 
    row = 0;
    for (i = 0; i < statbuf.st_size; i++) {
      if (input[i] == ' ') {
        row++;
      }
    }
    // EOF will be replaced by a space
    row++;
    
    data = (char**)malloc(row*sizeof(char*));
    if (isNULLPtr(data)) {
      free(input);
      exit(1);
    }

    i = 0;
    ptr = 0;
    isFound = 0;
    
    // assign addressed of words to data
    
    while (ptr != row) {
      if (!isFound) {
        data[ptr++] = &input[i];
        isFound = 1;
        if (input[i] != ' ') i++;
      } 
      else {
        if (input[i] == ' ') isFound = 0;
        i++;
      }
    }
    
    // replace EOF with space
    input[statbuf.st_size - 1] = ' ';
         
  } 
  // =============================
  // if input is a not file
  // allocate memory byte by byte
  // =============================
  else {

    char buf[1];
    int flag = read(0, buf, 1);
    if (flag == -1) {
      fprintf(stderr, "%s\n", "read error");
      exit(1);
    }

    // empty stdin
    if (flag == 0) return 0;

    data = (char**)malloc(sizeof(char*));
    if (isNULLPtr(data)) {
      exit(1);
    }

    data[0] = (char*)malloc(sizeof(char));
    if (isNULLPtr(data[0])) {
      free(data);
      exit(1);
    }

    row = 1;
    int col = 1;

    while (flag > 0) {

      if (buf[0] == ' ') {
        data[row - 1][col - 1] = ' ';
        char **temp = data;
        data = (char**)realloc(data, (row + 1)*sizeof(char*));
        if (isNULLPtr(data)) {
          freeData(temp, row);
          exit(1);
        }

        row++;
        
        data[row - 1] = (char*)malloc(sizeof(char));
        if (isNULLPtr(data[row - 1])) {
          freeData(data, row);
          exit(1);
        }

        col = 1;

      }
      else {

        data[row - 1][col - 1] = buf[0];
        char *temp = data[row - 1];
        data[row - 1] = (char*)realloc(data[row - 1], (col + 1)*sizeof(char));
        if (isNULLPtr(data[row - 1])) {
          free(temp);
          freeData(data, row - 1);
          exit(1);
        }

        col++;

      }

      flag = read(0, buf, 1);
      if (flag == -1) {
        fprintf(stderr, "%s\n", "read error");
        freeData(data, row);
        exit(1);
      }

    }

    data[row - 1][col - 1] = ' ';
  }
  
  // sort
  (f_option) ? qsort(data, row, sizeof(char*), frobcmpF) : qsort(data, row, sizeof(char*), frobcmp);

  // print
  int flag = printData(data, row);
  if (flag == -1) {
    if (S_ISREG(statbuf.st_mode)) {
      freeInput(data, input);
    } else {
      freeData(data, row);
    }
    exit(1);
  }
  

  // free data
  if (S_ISREG(statbuf.st_mode)) {
    freeInput(data, input);
  } else {
    freeData(data, row);
  }
  
  return 0;
}
