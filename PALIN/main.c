#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check(const char* x, int len) {
  for (int i = 0; i < len / 2; i++) {
    if (x[i] != x[len - i - 1]) {
      return 0;
    }
  }
  return 1;
}

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
    char* old = x;
    x = malloc((*len) + 2);
    x[0] = '1';
    for (int i = 0; i < *len; i++) {
      x[i + 1] = old[i];
    }
    free(old);
    (*len)++;
  }
  return x;
}

char* inc(char* x, size_t* len) {
  return inc_from(x, len, *len - 1);
}

int main(int argc, char** argv) {
  int count;
  scanf("%d\n", &count);
  for (int i = 0; i < count; i++) {
    char* num = malloc(1000001);
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
    int med = len / 2;
    int left = med - 1;
    int right = med + (len % 2);
    int increased = 0;
    while (left > -1 && right < len) {
      int leftNum = (int) (num[left] - '0');
      int rightNum = (int) (num[right] - '0');
      if (leftNum > rightNum) {
        num[right] = '0' + (char) leftNum;
        increased = 1;
      } else if (leftNum < rightNum) {
        if (increased) {
          num[right] = '0' + (char) leftNum;
        } else {
          num = inc_from(num, &len, med - 1 + (len % 2));
          left = med;
          right = med + (len % 2) - 1;
          increased = 1;
        }
      }
      left--;
      right++;
    }
    printf("%s\n", num);
  }

  return 0;
}

