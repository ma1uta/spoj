#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_LENGTH 1000001

// increment from specified position
char* inc_from(char* x, size_t* len, size_t from) {
  int shift = 0;
  for (int i = from; i > -1; i--) {
    int digit = (int) (x[i] - '0');
    if (digit < 9) {
      x[i] = '0' + (char) (digit + 1);
      shift = 0;
      break;
    } else {
      x[i] = '0';
      shift = 1;
    }
  }
  if (shift) {
    for (int i = *len - 1; i > -1; i--) {
      x[i + 1] = x[i];
    }
    x[0] = '1';
    (*len)++;
  }
  return x;
}

// increment
char* inc(char* x, size_t* len) {
  return inc_from(x, len, *len - 1);
}

int main(int argc, char** argv) {
  int count;
  scanf("%d\n", &count);
  char* num = malloc(MAX_NUMBER_LENGTH * sizeof(char));
  for (int i = 0; i < count; i++) {
    for (int j = 0; num[j] != '\0' || j < MAX_NUMBER_LENGTH; j++) {
      num[j] = '\0';
    }
    scanf("%s\n", num);
    size_t len = strlen(num);
    if (!len) {
      printf("1\n");
      continue;
    }
    num = inc(num, &len);
    if (len == 1) {
      printf("%s\n", num);
      continue;
    }
    // find a median
    int med = len / 2;
    // left pointer
    int left = med - 1;
    // right pointer
    int right = med + (len % 2);
    // flag is right part was increased
    int increased = 0;
    while (left > -1 && right < len) {
      // char at left pointer
      int leftNum = (int) (num[left] - '0');
      // char at right pointer
      int rightNum = (int) (num[right] - '0');
      // if left char greater than right
      if (leftNum > rightNum) {
        // that mirror left char and store that we increased right part
        num[right] = '0' + (char) leftNum;
        increased = 1;
      } else if (leftNum < rightNum) { // otherwise
        // if right part was increased
        if (increased) {
          // then mirror right part
          num[right] = '0' + (char) leftNum;
        } else {
          // else increment the median
          // and start again from median
          num = inc_from(num, &len, med - 1 + (len % 2));
          left = med;
          right = med + (len % 2) - 1;
          increased = 1;
        }
      }
      // move left pointer to left
      left--;
      // and right to right
      right++;
    }
    printf("%s\n", num);
  }
  free(num);

  return 0;
}

