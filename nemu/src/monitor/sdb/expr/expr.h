#pragma once
#include <debug.h>
#define MAX_TOKEN_STR_LEN 128
#define MAX_TOKEN_NUM 128

typedef struct token
{
  int type;
  char str[MAX_TOKEN_STR_LEN];
} Token;
void expr_test();
void single_test();
void bnf_expr_test();
