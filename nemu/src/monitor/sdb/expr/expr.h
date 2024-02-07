#pragma once
#include <debug.h>
#define MAX_TOKEN_STR_LEN 32

typedef struct token
{
  int type;
  char str[MAX_TOKEN_STR_LEN];
} Token;
void expr_test();
void single_test();
