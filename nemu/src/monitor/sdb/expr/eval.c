#include <common.h>
#include "string.h"
#include "expr.h"

extern Token tokens[];

bool check_parentheses(int32_t p, int32_t q)
{
  Token tokenP = tokens[p];
  Token tokenQ = tokens[q];
  if (tokenP.type == '(' && tokenQ.type == ')')
  {
    int count = 0;
    for (int32_t i = p + 1; i < q; i++)
    {
      Token token = tokens[i];
      if (token.type == '(')
        count++;
      if (token.type == ')')
        count--;
      if (count < 0)
        return false;
    }
    return count == 0;
  }
  return false;
}
/*
index 处的符号在[l_range,r_range]范围内是否在有效的括号内
*/

bool is_in_parentheses(int32_t index, int32_t l_range, int32_t r_range)
{
  int left_parentheses_count = 0;
  int right_parentheses_count = 0;
  for (int32_t i = index; i >= l_range; i--)
  {
    if (tokens[i].type == ')')
      left_parentheses_count--;
    if (tokens[i].type == '(')
      left_parentheses_count++;
  }
  for (int32_t i = index; i <= r_range; i++)
  {
    if (tokens[i].type == ')')
      right_parentheses_count++;
    if (tokens[i].type == '(')
      right_parentheses_count--;
  }

  return left_parentheses_count != 0 && left_parentheses_count == right_parentheses_count;
}
/*
 * [p,q] 里优先级最低的运算符
 * 非运算符的token不是主运算符
 * 出现在一对括号中的token不是主运算符
 * 主运算符的优先级在表达式中是最低的. 这是因为主运算符是最后一步才进行的运算符
 * 当有多个运算符的优先级都是最低时, 根据结合性, 最后被结合的运算符才是主运算符. 一个例子是1 + 2 + 3, 它的主运算符应该是右边的+.
 * */
int32_t find_main_operator_index(int32_t start, int32_t end)
{
  for (int32_t i = end; i >= start; i--)
  {
    if ((tokens[i].type == '+' || tokens[i].type == '-') && !is_in_parentheses(i, start, end))
      return i;
  }
  for (int32_t i = end; i >= start; i--)
  {
    if ((tokens[i].type == '*' || tokens[i].type == '/') && !is_in_parentheses(i, start, end))
      return i;
  }
  return -1;
}

word_t eval(uint32_t p, uint32_t q)
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
    return (int32_t)strtol((tokens[p]).str, NULL, 0);
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
    int32_t op = find_main_operator_index(p, q);
    int32_t val1 = eval(p, op - 1);
    int32_t val2 = eval(op + 1, q);
    int32_t op_type = (tokens[op]).type;
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