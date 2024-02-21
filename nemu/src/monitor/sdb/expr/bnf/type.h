#pragma once
typedef enum
{
  TK_REG,   // 寄存器
  TK_PUNCT, // 操作符如： + -
  TK_NUM,   // 数字
  TK_EOF,   // 文件终止符，即文件的最后
} TokenKind;

typedef struct BNFToken BNFToken;

struct BNFToken
{
  TokenKind kind; // 种类
  BNFToken *next; // 指向下一终结符
  int val;        // 值
  char *loc;      // 在解析的字符串内的位置
  int len;        // 长度
};

typedef enum
{
  ND_ADD,   // +
  ND_SUB,   // -
  ND_MUL,   // *
  ND_DIV,   // /
  ND_NEG,   // 负号-
  ND_EQ,    // ==
  ND_NE,    // !=
  ND_LT,    // <
  ND_LE,    // <=
  ND_DEREF, // 解引用 *
  ND_VAR,   // 变量
  ND_NUM,   // 数字
} NodeKind;

typedef struct Node Node;

struct Node
{
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};