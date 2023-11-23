#include <common.h>
#include "string.h"
#include "./header.h"

bool check_parentheses(uint32_t p, uint32_t q)
{
  if ((&tokens[p])->type == '(' && (&tokens[q])->type == ')')
  {
    int count = 0;
    for (size_t i = p + 1; i < q; i++)
    {
      if ((&tokens[p])->type == '(')
        count++;
      if ((&tokens[p])->type == ')')
        count--;
      if (count < 0)
        return false;
    }
    return count == 0;
  }
  return false;
}

bool is_in_parentheses(uint32_t l_range, uint32_t r_range, uint32_t index)
{
  uint32_t r_i = index;
  bool r_exist = false;
  while (r_i < r_range)
  {
    if ((&tokens[r_i])->type == ')')
    {
      r_exist = true;
      break;
    }
    r_i++;
  }
  uint32_t l_i = index;
  bool l_exist = false;
  while (l_i > r_range)
  {
    if ((&tokens[l_i])->type == '(')
    {
      l_exist = true;
      break;
    }
    l_i--;
  }
  return r_exist && l_exist;
}

int32_t find_main_operator_index(uint32_t p, uint32_t q)
{
  int32_t index1 = q;
  while (index1 >= p)
  {
    if (((&tokens[index1])->type == '+' || (&tokens[index1])->type == '-') && !is_in_parentheses(p, q, index1))
    {
      return index1;
    }
  }
  int32_t index2 = q;
  while (index2 >= p)
  {
    if (((&tokens[index2])->type == '+' || (&tokens[index2])->type == '-') && !is_in_parentheses(p, q, index2))
    {
      return index2;
    }
  }
  return -1;
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
    return (uint32_t)strtol((&tokens[p])->str, NULL, 0);
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
    uint32_t op = find_main_operator_index(p, q);
    uint32_t val1 = eval(p, op - 1);
    uint32_t val2 = eval(op + 1, q);
    uint32_t op_type = (&tokens[op])->type;
    switch (op_type)
    {
    case '+':
      return val1 + val2;
    case '-':
      return val1 - val2;
    case '*':
      return val1 * val2;
    case '/':
      return val1 / val2;
    default:
      assert(0);
    }
  }
}