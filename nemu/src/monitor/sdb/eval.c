#include <common.h>
#include "string.h"
bool check_parentheses(uint32_t p, uint32_t q)
{
  return false;
}
extern Token tokens;
Token find_main_operator(uint32_t p, uint32_t q)
{
  return tokens[1];
}

uint32_t eval(uint32_t p, uint32_t q)
{
  if (p > q)
  {
    /* Bad expression */
    Assert(0, "Invalid expression eval");
  }
  else if (p == q)
  {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    // q 右边要不就是空格，要不就是运算符
    return (uint32_t)strtol(tokens[p].str, NULL, 0);
  }
  else if (check_parentheses(p, q) == true)
  {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else
  {
    // op = the position of 主运算符 in the token expression;
    Token main_token = find_main_operator(p, q);
    char *op = main_token.str;
    int op_type = main_token.type;
    uint32_t val1 = eval(p, op - 1);
    uint32_t val2 = eval(op + 1, q);
    tokens switch (op_type)
    {
    case '+':
      return val1 + val2;
    case '-': /* ... */
    case '*': /* ... */
    case '/': /* ... */
    default:
      assert(0);
    }
  }
}