#pragma once
typedef struct token
{
  int type;
  char str[32];
} Token;
void expr_test();
void single_test();
