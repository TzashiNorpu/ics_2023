#include <stdlib.h>
#include "utils.h"
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "error-output.h"
#include "tokenize.h"
#include <isa.h>
#include <debug.h>

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

bool token_equal(BNFToken *token, char *op)
{
  if (token->len != strlen(op))
    return false;
  return strncmp(token->loc, op, token->len) == 0;
}

// 操作符判断
void get_punct_len(char *ptr, bool *is_punct, int *punct_len)
{
  // 判断2字节的操作符
  if (start_with(ptr, "==") || start_with(ptr, "!=") || start_with(ptr, "<=") || start_with(ptr, ">=") || start_with(ptr, "&&"))
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

// 读取数字字面量
BNFToken *read_literal_num(char *loc)
{
  char *p = loc;

  // 读取二、八、十、十六进制
  // 默认为十进制
  int base = 10;
  // 比较两个字符串前2个字符，忽略大小写，并判断是否为数字
  if (!strncasecmp(p, "0x", 2) && isxdigit(p[2]))
  {
    // 十六进制
    p += 2;
    base = 16;
  }
  else if (!strncasecmp(p, "0b", 2) && (p[2] == '0' || p[2] == '1'))
  {
    // 二进制
    p += 2;
    base = 2;
  }
  else if (*p == '0')
  {
    // 八进制
    base = 8;
  }

  // 将字符串转换为Base进制的数字
  // strtoul 会移动 p 指针
  long val = strtoul(p, &p, base);
  if (isalnum(*p))
    errorAt(p, "invalid digit");

  // 构造NUM的终结符
  BNFToken *tok = new_token(TK_NUM, loc, p);
  tok->val = val;
  return tok;
}

BNFToken *get_reg_token(char *loc, int len)
{
  char *reg_name = NULL;
  strncpy(reg_name, loc, len);
  bool success = false;
  long val = isa_reg_str2val(reg_name, &success);
  // success 为 false 时触发
  Assert(success, "get reg value error");
  BNFToken *tok = new_token(TK_NUM, loc, loc + len);
  tok->val = val;
  return tok;
}
