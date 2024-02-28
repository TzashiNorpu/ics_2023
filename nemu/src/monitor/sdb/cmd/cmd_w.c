#include "cmd_w.h"
#include "../watchpoint/watchpoint.h"
#include "../expr/bnf/expr.h"
int cmd_w(char *p)
{
  long val = calc(p);
  WP *point = new_wp();
  char *expr = calloc(1, strlen(p));
  strcpy(expr, p);
  point->expr = expr;
  point->val = val;
  return 0;
}