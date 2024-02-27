#include "parse.h"
#include <stdlib.h>
#include "error-output.h"
#include "utils.h"
extern BNFToken *token;

bool consume(char *op)
{
  if (token->kind != TK_PUNCT || !token_equal(token, op))
    return false;
  token = token->next;
  return true;
}

void expect(char *op)
{
  if (token->kind != TK_PUNCT || !token_equal(token, op))
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

Node *new_unary_node(NodeKind kind, Node *lhs)
{
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  return node;
}

/*
  推导式
*/
// expr = equality
// equality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add = mul ("+" mul | "-" mul)*
// mul = unary ("*" unary | "/" unary)*
// unary = ("+" | "-" | "*") unary | primary
// primary = "(" expr ")" | num

Node *bnf_expr()
{
  return equality();
}

Node *equality()
{
  Node *node = relational();

  for (;;)
  {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node(ND_NE, node, relational());
    else
      return node;
  }
}

Node *relational()
{
  Node *node = add();

  for (;;)
  {
    if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume(">"))
      node = new_node(ND_LT, add(), node);
    else if (consume("<="))
      node = new_node(ND_LE, node, add());
    else if (consume(">="))
      node = new_node(ND_LE, add(), node);
    else
      return node;
  }
}

Node *add()
{
  Node *node = mul();

  for (;;)
  {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul()
{
  Node *node = unary();

  for (;;)
  {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary()
{
  if (consume("+"))
    return unary(); // 调用自身：实现跳过的目的
  else if (consume("-"))
    return new_unary_node(ND_NEG, unary());
  else if (consume("*"))
    return new_unary_node(ND_DEREF, unary());
  else
    return primary();
}

Node *primary()
{
  if (consume("("))
  {
    Node *node = bnf_expr();
    expect(")");
    return node;
  }

  return new_node_num(expect_number());
}