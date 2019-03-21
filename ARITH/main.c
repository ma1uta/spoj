#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DIGIT 501
#define MAX_EXPRESSION 1001
#define MAX(x,y) ((x) > (y) ? (x) : (y))

struct expression_t {
    char arg1[MAX_DIGIT];
    char arg2[MAX_DIGIT];
    char operator;
};

struct result {
    struct expression_t* exp;
    size_t res_count;
    char** res;
};

void calc(const char* input);

int main()
{
    int t;
    scanf("%d\n", &t);

    char input[MAX_EXPRESSION];
    for (int i = 0; i < t; i++) {
        scanf("%s\n", input);
        calc(input);
    }

    return 0;
}

void plus(struct result* res);
void minus(struct result* res);
void mult(struct result* res);
void print(const struct result* res);

void calc(const char* input)
{
    struct expression_t exp;

    size_t i = 0;
    for (;input[i] != '+' && input[i] != '-' && input[i] != '*'; i++)
    {
        exp.arg1[i] = input[i];
    }
    exp.arg1[i] = '\0';
    exp.operator = input[i];
    size_t j = ++i;
    for (; input[i] != '\0'; i++)
    {
        exp.arg2[i - j] = input[i];
    }
    exp.arg2[i - j] = '\0';

    struct result* res = malloc(sizeof (struct result));
    res->exp = &exp;
    switch (exp.operator)
    {
    case '+':
        plus(res);
        break;
    case '-':
        minus(res);
        break;
    case '*':
        mult(res);
        break;
    default:
        return;
    }
    print(res);
    for (size_t i = 0; i < res->res_count; i++)
    {
        free(res->res[i]);
    }
    free(res->res);
    free(res);
}

void plus(struct result* res)
{
    size_t len1 = strlen(res->exp->arg1);
    size_t len2 = strlen(res->exp->arg2);
    size_t len = MAX(len1, len2);
    res->res_count = 1;
    res->res = malloc(sizeof (char*));

    char* inv_res = malloc(sizeof (char) * (len + 2));
    size_t shift = 0;
    size_t i = 0;
    while (i < len1 || i < len2)
    {
        char c1 = (i < len1) ? res->exp->arg1[len1 - i - 1] : '0';
        char c2 = (i < len2) ? res->exp->arg2[len2 - i - 1] : '0';
        unsigned long d1 = (unsigned long) (c1 - '0');
        unsigned long d2 = (unsigned long) (c2 - '0');
        char sum_str[3];
        unsigned long sum = d1 + d2 + shift;
        snprintf(sum_str, 3, "%ld", (d1 + d2 + shift));
        if (sum > 9)
        {
            inv_res[i] = sum_str[1];
            shift = 1;
        }
        else
        {
            inv_res[i] = sum_str[0];
            shift = 0;
        }
        i++;
    }
    if (shift)
    {
        inv_res[i] = '1';
    }
    res->res[0] = malloc(sizeof (char) * (len + shift));
    for (size_t j = 0; j < len + shift; j++)
    {
        res->res[0][j] = inv_res[len + shift - j - 1];
    }
    res->res[0][len + shift] = '\0';
    free(inv_res);
}

void minus(struct result* res)
{
    size_t len1 = strlen(res->exp->arg1);
    size_t len2 = strlen(res->exp->arg2);
    size_t len = MAX(len1, len2);
    res->res_count = 1;
    res->res = malloc(sizeof (char*));

    res->res[0] = malloc(sizeof (char) * (len + 1));

    size_t shift = 0;
    size_t i = 0;
    while (i < len1)
    {
        char c1 = (i < len1) ? res->exp->arg1[len1 - i - 1] : '0';
        char c2 = (i < len2) ? res->exp->arg2[len2 - i - 1] : '0';
        unsigned long d1 = (unsigned long) (c1 - '0');
        unsigned long d2 = (unsigned long) (c2 - '0');
        char sum_str[3];
        long sum = d1 - d2 - shift;
        snprintf(sum_str, 3, "%ld", (d1 + d2 + shift));
        if (sum > 9)
        {
            inv_res[i] = sum_str[1];
            shift = 1;
        }
        else
        {
            inv_res[i] = sum_str[0];
            shift = 0;
        }
        i++;
    }
    if (shift)
    {
        inv_res[i] = '1';
    }
    res->res[0] = malloc(sizeof (char) * (len + shift));
    for (size_t j = 0; j < len + shift; j++)
    {
        res->res[0][j] = inv_res[len + shift - j - 1];
    }
    res->res[0][len + shift] = '\0';
    free(inv_res);
}

void mult(struct result* res)
{
}

void print(const struct result* res)
{
    size_t len1 = strlen(res->exp->arg1);
    size_t len2 = strlen(res->exp->arg2);
    size_t max = MAX(len1, len2 + 1);
    for (size_t i = 0; i < res->res_count; i++)
    {
        max = MAX(max, strlen(res->res[i]));
    }

    for (size_t i = 0; i < max - len1; i++)
    {
        fputc(' ', stdout);
    }
    fputs(res->exp->arg1, stdout);
    fputc('\n', stdout);

    for (size_t i = 0; i < max - len2 - 1; i++)
    {
        fputc(' ', stdout);
    }
    fputc(res->exp->operator, stdout);
    fputs(res->exp->arg2, stdout);
    fputc('\n', stdout);

    for (size_t i = 0; i < max; i++)
    {
        fputc('-', stdout);
    }
    fputc('\n', stdout);

    for (size_t i = 0; i < res->res_count; i++)
    {
        size_t len = strlen(res->res[i]);
        for (size_t j = 0; j < max - len - i; j++)
        {
            fputc(' ', stdout);
        }
        fputs(res->res[i], stdout);
        fputc('\n', stdout);
    }
}
