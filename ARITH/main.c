#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DIGIT 501
#define MAX_EXPRESSION 1001
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

struct expression_t {
    char arg1[MAX_DIGIT];
    char arg2[MAX_DIGIT];
    char operator;
    char gap_0[5]; // not used
    size_t len1;
    size_t len2;
    size_t res_count;
    char** res;
};

void calc(const char* input);

int main() {
    int t;
    scanf("%d\n", &t);

    char input[MAX_EXPRESSION];
    for (int i = 0; i < t; i++) {
        scanf("%s\n", input);
        calc(input);
    }

    return 0;
}

void plus(struct expression_t* exp);
void minus(struct expression_t* exp);
void mult(struct expression_t* exp);
void print(const struct expression_t* exp);
void trim(struct expression_t* exp, size_t line, size_t max);

void calc(const char* input) {
    struct expression_t exp;

    size_t i;
    size_t j;
    size_t len = strlen(input);
    for (j = 0, i = len - 1; input[i] != '+' && input[i] != '-' && input[i] != '*'; i--, j++) {
        exp.arg2[j] = input[i];
    }
    exp.arg1[j + 1] = '\0';
    exp.len2 = j;
    exp.operator = input[i];
    for (j = 0; j < i; j++) {
        exp.arg1[j] = input[i - j - 1];
    }
    exp.arg1[j] = '\0';
    exp.len1 = j;

    switch (exp.operator) {
    case '+':
        plus(&exp);
        break;
    case '-':
        minus(&exp);
        break;
    case '*':
        mult(&exp);
        break;
    default:
        return;
    }
    print(&exp);
    for (size_t i = 0; i < exp.res_count; i++) {
        free(exp.res[i]);
    }
    free(exp.res);
}

void plus(struct expression_t* exp) {
    size_t len = MAX(exp->len1, exp->len2);
    exp->res_count = 1;
    exp->res = malloc(sizeof (char*));

    exp->res[0] = malloc(sizeof (char) * (len + 2));
    size_t shift = 0;
    size_t i = 0;
    while (i < exp->len1 || i < exp->len2) {
        char c1 = (i < exp->len1) ? exp->arg1[i] : '0';
        char c2 = (i < exp->len2) ? exp->arg2[i] : '0';
        unsigned long d1 = (unsigned long) (c1 - '0');
        unsigned long d2 = (unsigned long) (c2 - '0');
        unsigned long sum = d1 + d2 + shift;
        if (sum > 9) {
            shift = sum / 10;
            sum %= 10;
        } else {
            shift = 0;
        }
        exp->res[0][i] = (char) sum + '0';
        i++;
    }
    if (shift != 0) {
        exp->res[0][i++] = '1';
    }
    exp->res[0][i] = '\0';
    trim(exp, 0, i);
}

void minus(struct expression_t* exp) {
    size_t len = MAX(exp->len1, exp->len2);
    exp->res_count = 1;
    exp->res = malloc(sizeof (char*));
    exp->res[0] = malloc(sizeof (char) * (len + 1));

    long shift = 0;
    size_t i = 0;
    while (i < exp->len1) {
        char c1 = (i < exp->len1) ? exp->arg1[i] : '0';
        char c2 = (i < exp->len2) ? exp->arg2[i] : '0';
        long d1 = (long) (c1 - '0');
        long d2 = (long) (c2 - '0');
        long sub = d1 - d2 - shift;
        if (sub < 0) {
            sub = (sub + 10) % 10;
            shift = 1;
        } else {
            shift = 0;
        }
        exp->res[0][i] = (char) sub + '0';
        i++;
    }
    exp->res[0][exp->len1] = '\0';
    trim(exp, 0, exp->len1);
}

void mult(struct expression_t* exp) {
    unsigned long mem = 0;
    exp->res_count = exp->len2 + 1;
    exp->res = malloc(sizeof (char*) * exp->res_count);
    for (unsigned long i = 0; i < exp->len2; i++) {
        unsigned long m2 = (unsigned long) (exp->arg2[i] - '0');
        if (m2 == 0) {
            exp->res[i] = malloc(sizeof (char) * 2);
            exp->res[i][0] = '0';
            exp->res[i][1] = '\0';
        } else {
            exp->res[i] = malloc(sizeof (char) * (exp->len1 + 2));
            size_t j;
            for (j = 0; j < exp->len1; j++) {
                unsigned long m1 = (unsigned long) (exp->arg1[j] - '0');
                unsigned long mult = m1 * m2 + mem;
                mem = mult / 10;
                unsigned long res = mult % 10;
                exp->res[i][j] = (char) res + '0';
            }
            while (mem != 0) {
                unsigned long res = mem % 10;
                exp->res[i][j++] = (char) res + '0';
                mem = mem / 10;
            }
            exp->res[i][j] = '\0';
            trim(exp, i, j);
        }
    }
    size_t max = 0;
    size_t* lengths = malloc(sizeof (size_t) * (exp->res_count - 1));
    for (size_t i = 0; i < exp->res_count - 1; i++) {
        lengths[i] = strlen(exp->res[i]);
        max = MAX(max, lengths[i] + i);
    }
    exp->res[exp->res_count - 1] = malloc(sizeof (char) * (max + 2));
    unsigned long offset = 0;
    for (size_t i = 0; i < max; i++) {
        unsigned long sum = 0;
        size_t min = MIN(i, exp->res_count - 2);
        for (size_t j = 0; j <= min; j++) {
            if (lengths[j] + j > i) {
                sum += (unsigned long) (exp->res[j][i - j] - '0');
            }
        }
        sum += offset;
        if (sum > 9) {
            offset = sum / 10;
            sum %= 10;
        } else {
            offset = 0;
        }
        exp->res[exp->res_count - 1][i] = (char) sum + '0';
    }
    if (offset > 0) {
        exp->res[exp->res_count - 1][max] = (char) offset + '0';
        exp->res[exp->res_count - 1][++max] = '\0';
    } else {
        exp->res[exp->res_count - 1][max] = '\0';
    }
    trim(exp, exp->res_count - 1, max);
    free(lengths);
}

void trim(struct expression_t* exp, size_t line, size_t max)
{
    for (size_t i = 0; i < max - 1; i++) {
        if (exp->res[line][max - i - 1] == '0') {
            exp->res[line][max - i - 1] = '\0';
        } else {
            break;
        }
    }
}

void print(const struct expression_t* exp) {
    size_t max = MAX(exp->len1, exp->len2 + 1);
    for (size_t i = 0; i < exp->res_count - 1; i++) {
        max = MAX(max, strlen(exp->res[i]) + i);
    }
    max = MAX(max, strlen(exp->res[exp->res_count - 1]));

    for (size_t i = 0; i < max - exp->len1; i++) {
        fputc(' ', stdout);
    }
    for (size_t i = 0; i < exp->len1; i++) {
        fputc(exp->arg1[exp->len1 - i - 1], stdout);
    }
    fputc('\n', stdout);

    for (size_t i = 0; i < max - exp->len2 - 1; i++) {
        fputc(' ', stdout);
    }
    fputc(exp->operator, stdout);
    for (size_t i = 0; i < exp->len2; i++) {
        fputc(exp->arg2[exp->len2 - i - 1], stdout);
    }
    fputc('\n', stdout);

    size_t max1 = MAX(exp->len2 + 1, strlen(exp->res[0]));
    for (size_t i = 0; i < max; i++) {
        fputc(i < max - max1 ? ' ' : '-', stdout);
    }
    fputc('\n', stdout);

    for (size_t i = 0; i < exp->res_count - 1; i++) {
        size_t len = strlen(exp->res[i]);
        for (size_t j = 0; j < max - len - i; j++) {
            fputc(' ', stdout);
        }
        for (size_t j = 0; j < len; j++) {
            fputc(exp->res[i][len - j - 1], stdout);
        }
        fputc('\n', stdout);
    }
    if (exp->operator == '*' && exp->res_count > 2) {
        size_t max2 = MAX(strlen(exp->res[exp->res_count - 1]), strlen(exp->res[exp->res_count - 2]));
        for (size_t i = 0; i < max - max2; i++) {
            fputc(' ', stdout);
        }
        for (size_t i = 0; i < max2; i++) {
            fputc('-', stdout);
        }
        fputc('\n', stdout);
    }
    if (exp->operator != '*' || exp->res_count > 2) {
        size_t len = strlen(exp->res[exp->res_count - 1]);
        if (len < max) {
            for (size_t i = 0; i < max - len; i++) {
                fputc(' ', stdout);
            }
        }
        for (size_t i = 0; i < len; i++) {
            fputc(exp->res[exp->res_count - 1][len - i -1], stdout);
        }
        fputc('\n', stdout);
    }
    fputc('\n', stdout);
}
