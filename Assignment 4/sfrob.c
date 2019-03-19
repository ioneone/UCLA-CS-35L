#include <stdio.h>
#include <stdlib.h>

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

void freeData(char **data, int row) {
  // free memory allocation
  int i;
  for (i = 0; i < row; i++) {
    free(data[i]);
  }
  free(data);
}

int isIOError() {
  if (ferror(stdin) || ferror(stdout)) {
    fprintf(stderr, "IO Error\n");
    return 1;
  }

  return 0;
}

int isNULLPtr(void *ptr) {
  if (ptr == NULL) {
    fprintf(stderr, "Memory Allocation Error\n");
    return 1;
  }

  return 0;
}

void printData(char **data, int row) {
  int i;
  for (i = 0; i < row; i++) {
    int j = 0;
    while (data[i][j] != ' ') { 
      printf("%c", data[i][j++]);
      if (isIOError()) {
        freeData(data, row);
        exit(1);
      }
    }
    
    printf(" ");
    if (isIOError()) {
      freeData(data, row);
      exit(1);
    }
  }
  
}

int main() {
  char **data;
  char c;
  
  data = (char**)malloc(sizeof(char*));
  if (isNULLPtr(data)) {
    exit(1);
  }

  data[0] = (char*)malloc(sizeof(char));
  if (isNULLPtr(data[0])) {
    free(data);
    exit(1);
  }

  int row = 0; // index
  int col = 0; // index

  c = getchar();
  if (isIOError()) {
    freeData(data, row + 1);
    exit(1);
  }

  while (!feof(stdin)) {
    
    if (c == ' ') {
      data[row++][col] = c;
      col = 0;
      char **tmp = data;
      data = (char**)realloc(data, (row+1)*sizeof(char*));
      if (isNULLPtr(data)) {                                                             
        freeData(tmp, row + 1);                        
        exit(1);                                                                        
      }

      data[row] = (char*)malloc(sizeof(char));
      if (isNULLPtr(data[row])) {                                                          
        freeData(data, row + 1);                      
        exit(1);                                                                       
      }                                                                               
    }
    else {
      data[row][col++] = c;
      char *tmp = data[row];
      data[row] = (char*)realloc(data[row], (col+1)*sizeof(char));
      if (isNULLPtr(data[row])) {                                                          
        free(tmp);
        freeData(data, row + 1);                         
        exit(1);                                                                        
      }                                                                               
    }

    c = getchar();
    if (isIOError()) {
      freeData(data, row + 1);
      exit(1);
    }

  }

  data[row][col] = ' ';

  
  // sort
  qsort(data, row + 1, sizeof(char*), frobcmp);

  // print
  printData(data, row + 1);

  // free data
  freeData(data, row + 1);
  
  return 0;
}