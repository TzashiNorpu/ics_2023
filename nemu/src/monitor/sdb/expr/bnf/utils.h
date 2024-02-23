#pragma once
#include <stdbool.h>
bool start_with(char *str, char *sub_str);
void get_punct_len(char *ptr, bool *is_punct, int *punct_len);
bool equal(char *str1, char *str2);