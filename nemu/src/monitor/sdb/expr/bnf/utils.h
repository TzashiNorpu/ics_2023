#pragma once
#include <stdbool.h>
#include "type.h"
bool start_with(char *str, char *sub_str);
void get_punct_len(char *ptr, bool *is_punct, int *punct_len);
bool equal(char *str1, char *str2);
bool token_equal(BNFToken *token, char *op);
BNFToken *read_literal_num(char *loc);
BNFToken *get_reg_token(char *loc, int len);