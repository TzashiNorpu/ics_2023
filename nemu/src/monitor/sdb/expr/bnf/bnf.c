#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bnf.h"

typedef enum
{
  TK_PUNCT, // 操作符如： + -
  TK_NUM,   // 数字
  TK_EOF,   // 文件终止符，即文件的最后
} TokenKind;

typedef struct Token Token;

struct Token
{
  TokenKind kind; // 种类
  Token *next;    // 指向下一终结符
  int val;        // 值
  char *loc;      // 在解析的字符串内的位置
  int len;        // 长度
};

typedef enum
{
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // 整数
} NodeKind;

typedef struct Node Node;

struct Node
{
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

Node *expr();
Node *mul();
Node *primary();
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
void error(char *fmt, ...);
void errorAt(char *loc, char *fmt, ...);
void verrorAt(char *loc, char *fmt, va_list VA);
void errorTok(Token *tok, char *fmt, ...);
bool consume(char op);
void expect(char op);
bool at_eof();
Token *new_token(TokenKind kind, char *start, char *end);
int preOrder(Node *node);

Token *token;

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

bool at_eof()
{
  return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, char *start, char *end)
{
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

Token *tokenize(char *p)
{
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p)
  {
    if (isspace(*p))
    {
      p++;
      continue;
    }

    if (isdigit(*p))
    {
      cur->next = new_token(TK_NUM, p, p);
      // 此时 cur 等于 new_token 新建出来的 token
      cur = cur->next;
      const char *OldPtr = p;
      // p 移动了，因此不需要再次移动
      cur->val = strtoul(p, &p, 10);
      cur->len = p - OldPtr;
      continue;
    }
    if (ispunct(*p))
    {
      cur->next = new_token(TK_PUNCT, p, p + 1);
      cur = cur->next;
      p++;
      continue;
    }

    errorAt(p, "invalid token");
  }

  cur->next = new_token(TK_EOF, p, p);
  return head.next;
}

// 输出错误信息
// fmt为传入的字符串， ... 为可变参数，表示fmt后面所有的参数
void error(char *fmt, ...)
{
  // 定义一个va_list变量
  va_list VA;
  // VA获取Fmt后面的所有参数
  va_start(VA, fmt);
  // vfprintf可以输出va_list类型的参数
  vfprintf(stderr, fmt, VA);
  // 在结尾加上一个换行符
  fprintf(stderr, "\n");
  // 清除VA
  va_end(VA);
  // 终止程序
  exit(1);
}
char *user_input;

// 输出错误出现的位置
void verrorAt(char *loc, char *fmt, va_list VA)
{
  // 先输出源信息
  fprintf(stderr, "%s\n", user_input);

  // 输出出错信息
  // 计算出错的位置，Loc是出错位置的指针，CurrentInput是当前输入的首地址
  int pos = loc - user_input;
  // 将字符串补齐为Pos位，因为是空字符串，所以填充Pos个空格。
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, VA);
  fprintf(stderr, "\n");
  va_end(VA);
}

// 字符解析出错，并退出程序
void errorAt(char *loc, char *fmt, ...)
{
  va_list VA;
  va_start(VA, fmt);
  verrorAt(loc, fmt, VA);
  exit(1);
}

// Tok解析出错，并退出程序
void errorTok(Token *tok, char *fmt, ...)
{
  va_list VA;
  va_start(VA, fmt);
  verrorAt(tok->loc, fmt, VA);
  exit(1);
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

Node *expr()
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
    Node *node = expr();
    expect(')');
    return node;
  }

  return new_node_num(expect_number());
}

int preOrder(Node *node)
{
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
  default:
    error("Unexpected operator:'%c'", node->kind);
  }
  return NULL;
}

int calc(char *str)
{
  user_input = str;
  token = tokenize(user_input);

  // AST
  Node *node = expr();

  // preOrder and calc
  return preOrder(node);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    error("wrong argc");
    return 1;
  }

  printf("res=%d\n", calc(argv[1]));
  return 0;
}