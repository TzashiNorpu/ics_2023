#include "expr.h"
#include "type.h"
#include "error-output.h"
#include "tokenize.h"
#include "parse.h"
int preOrder(Node *node)
{
  if (!node)
    return 0;
  if (node->kind == ND_NUM)
    return node->val;

  int l = preOrder(node->lhs);
  int r = preOrder(node->rhs);

  switch (node->kind)
  {
  case ND_ADD:
    return l + r;
  case ND_SUB:
    return l - r;
  case ND_MUL:
    return l * r;
  case ND_DIV:
    return l / r;
  case ND_NEG:
    return -1 * l + r;
  default:
    error("Unexpected operator:'%c'", node->kind);
  }
  error("error");
  return -1;
}
BNFToken *token;
char *user_input;

int calc(char *str)
{
  user_input = str;
  token = tokenize(user_input);

  // AST
  Node *node = bnf_expr();

  // preOrder and calc
  return preOrder(node);
}