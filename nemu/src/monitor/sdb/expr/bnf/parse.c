#include "parse.h"
#include <stdlib.h>
#include "error-output.h"
extern BNFToken *token;

bool consume(char op)
{
  if (token->kind != TK_PUNCT || token->loc[0] != op)
    return false;
  token = token->next;
  return true;
}

void expect(char op)
{
  if (token->kind != TK_PUNCT || token->loc[0] != op)
    error("'%c' is not equal token op", op);
  token = token->next;
}

int expect_number()
{
  if (token->kind != TK_NUM)
    errorTok(token, "expect a number");
  int val = token->val;
  token = token->next;
  return val;
}

Node *
new_node(NodeKind kind, Node *lhs, Node *rhs)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *bnf_expr()
{
  Node *node = mul();

  for (;;)
  {
    if (consume('+'))
      node = new_node(ND_ADD, node, mul());
    else if (consume('-'))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul()
{
  Node *node = primary();

  for (;;)
  {
    if (consume('*'))
      node = new_node(ND_MUL, node, primary());
    else if (consume('/'))
      node = new_node(ND_DIV, node, primary());
    else
      return node;
  }
}

Node *primary()
{
  if (consume('('))
  {
    Node *node = bnf_expr();
    expect(')');
    return node;
  }

  return new_node_num(expect_number());
}