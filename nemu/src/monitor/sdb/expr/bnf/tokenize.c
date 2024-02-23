#include "tokenize.h"
#include "error-output.h"
#include <stdlib.h>
#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
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
      const char *old_ptr = p;
      // p 移动了，因此不需要再次移动
      cur->val = strtoul(p, &p, 10);
      cur->len = p - old_ptr;
      continue;
    }

    bool is_punct = false;
    int punct_len = 0;
    get_punct_len(p, &is_punct, &punct_len);

    if (is_punct)
    {
      cur->next = new_token(TK_PUNCT, p, p + punct_len);
      cur = cur->next;
      p += punct_len;
      continue;
    }

    errorAt(p, "invalid token");
  }

  cur->next = new_token(TK_EOF, p, p);
  return head.next;
}