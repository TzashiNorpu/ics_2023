#include "tokenize.h"
#include "error-output.h"
#include <stdlib.h>
#include <ctype.h>
BNFToken *new_token(TokenKind kind, char *start, char *end)
{
  BNFToken *tok = calloc(1, sizeof(BNFToken));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

BNFToken *tokenize(char *p)
{
  BNFToken head;
  head.next = NULL;
  BNFToken *cur = &head;

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