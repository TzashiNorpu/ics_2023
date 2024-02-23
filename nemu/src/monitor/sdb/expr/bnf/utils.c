#include <stdlib.h>
#include "utils.h"
// 判断 str 是否以 sub_str 开头
bool start_with(char *str, char *sub_str)
{
  // 比较LHS和RHS的N个字符是否相等
  return strncmp(str, sub_str, strlen(sub_str)) == 0;
}

bool equal(char *str1, char *str2)
{
  if (strlen(str1) != strlen(str2))
    return false;
  return strcmp(str1, str2) == 0;
}
// 操作符判断
void get_punct_len(char *ptr, bool *is_punct, int *punct_len)
{
  // 判断2字节的操作符
  if (start_with(ptr, "==") || start_with(ptr, "!=") || start_with(ptr, "<=") || start_with(ptr, ">="))
  {

    *punct_len = 2;
    *is_punct = true;
    return;
  }

  if (ispunct(*ptr))
  {
    *punct_len = 1;
    *is_punct = true;
    return;
  }
}