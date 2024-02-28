#include "cmd_p.h"
#include <stdio.h>
#include "../expr/bnf/expr.h"
int cmd_p(char *p)
{
  long value = calc(p);
  printf("%s = %lu \n", p, value);
  return 0;
}