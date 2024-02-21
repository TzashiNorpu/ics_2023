#pragma once
#include "type.h"
#include <stdbool.h>


Node *bnf_expr();
Node *mul();
Node *primary();
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);

bool consume(char op);
void expect(char op);
int preOrder(Node *node);