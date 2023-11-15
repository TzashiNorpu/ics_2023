bool check_parentheses(char *p, char *q)
{

  return false;
}

uint32_t eval(char *p, char *q)
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
    return (uint32_t)atoi(*p);
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
    char *op = NULL;
    val1 = eval(p, op - 1);
    val2 = eval(op + 1, q);

    switch (op_type)
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