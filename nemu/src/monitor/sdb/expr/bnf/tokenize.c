#include "tokenize.h"
#include "error-output.h"
#include <stdlib.h>
#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <isa.h>
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
      /*
      cur->next = new_token(TK_NUM, p, p);
      // 此时 cur 等于 new_token 新建出来的 token
      cur = cur->next;
      const char *old_ptr = p;
      // p 移动了，因此不需要再次移动
      cur->val = strtoul(p, &p, 10);
      cur->len = p - old_ptr;
       */
      // const char *old_ptr = p;
      // read_literal_num 复制了 p ：没有移动本来的 p
      cur->next = read_literal_num(p);
      cur = cur->next;
      p += cur->len;
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

    if (start_with(p, "$"))
    {
      bool is_reg = false;
      int reg_len = 0;
      reg_info(p + 1, &is_reg, &reg_len);
      if (is_reg)
      {
        p++; // 如果是寄存器，则舍去 $
        cur->next = get_reg_token(p, reg_len);
        cur = cur->next;
        p += reg_len;
        continue;
      }
    }

    errorAt(p, "invalid token");
  }

  cur->next = new_token(TK_EOF, p, p);
  return head.next;
}