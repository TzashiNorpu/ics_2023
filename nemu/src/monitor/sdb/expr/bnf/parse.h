#pragma once
#include "type.h"
#include <stdbool.h>

Node *bnf_expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_unary_node(NodeKind kind, Node *lhs);

bool consume(char *op);
void expect(char *op);
int preOrder(Node *node);