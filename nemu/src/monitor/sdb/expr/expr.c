/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include "expr.h"
#include <stdlib.h>

enum
{
  TK_NOTYPE = 256,
  TK_EQ,

  /* TODO: Add more token types */
  TK_INT,
  TK_DEREF

};

static struct rule
{
  const char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE}, // spaces
    {"\\+", '+'},      // plus
    {"==", TK_EQ},     // equal
    // add
    {"\\-", '-'},       // minus
    {"\\*", '*'},       // multiply
    {"\\*", TK_DEREF},  // deref
    {"\\/", '/'},       // divide
    {"[0-9]+", TK_INT}, // int number
    {"\\(", '('},
    {"\\)", ')'},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++)
  {
    // 将 rules[i] 的 regex【" +"】 字符串编译到 re[i] 中
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0)
    {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

// typedef struct token
// {
//   int type;
//   char str[32];
// } Token;

// static Token tokens[32] __attribute__((used)) = {};
Token tokens[MAX_TOKEN_NUM] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(char *e)
{
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0')
  {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++)
    {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
      {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        // 只支持这么多的 token
        Assert(nr_token < MAX_TOKEN_NUM, "token 数目超出限制");

        switch (rules[i].token_type)
        {
        case TK_NOTYPE:
          break;
        case TK_INT:
          Token t1 = {};
          strncpy(t1.str, substr_start, substr_len);
          // 只支持这么长位数的整型
          Assert(substr_len < MAX_TOKEN_STR_LEN, "整型位数超长");
          t1.str[substr_len] = '\0';
          t1.type = rules[i].token_type;
          tokens[nr_token++] = t1;
          break;
        default:
          Token t2 = {};
          t2.type = rules[i].token_type;
          tokens[nr_token++] = t2;
          break;
        }

        break;
      }
    }

    if (i == NR_REGEX)
    {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool is_operator(int type)
{
  if (type == '+')
    return true;
  if (type == '-')
    return true;
  if (type == '*')
    return true;
  if (type == '/')
    return true;
  return false;
}

word_t eval(uint32_t p, uint32_t q);
word_t expr(char *e, bool *success)
{
  if (!make_token(e))
  {
    *success = false;
    return 0;
  }

  for (int i = 0; i < nr_token; i++)
    if (tokens[i].type == '*' && (i == 0 || is_operator(tokens[i - 1].type)))
      tokens[i].type = TK_DEREF;
  return eval(0, nr_token - 1);
}

void expr_test()
{
  int match_count = 0;
  int no_match_count = 0;
  char *NEMU_HOME = getenv("NEMU_HOME");
  char *filename = strcat(NEMU_HOME, "/tools/gen-expr/input.txt");
  printf("filename = %s\n", filename);
  FILE *fp = fopen(filename, "r");
  assert(fp != NULL);

  char *line = NULL;
  size_t len = 0;
  size_t nread = 0;
  while ((nread = getline(&line, &len, fp)) != -1)
  {
    /*  unsigned exp_value;
      char exp[nread];
      int tmp = sscanf(line, "%u %s", &exp_value, exp);
      assert(tmp == 2);*/

    char *token = strtok(line, " ");
    unsigned exp_value = atoi(token);
    char *exp = strtok(NULL, " ");
    // 移除换行符
    int str_size = strlen(exp);
    exp[str_size - 1] = '\0';

    Log("Retrieved test line content: res=%u,exp=%s", exp_value, exp);
    bool expr_parse_success = true;
    word_t expr_parse_value = expr(exp, &expr_parse_success);
    Log("Expr_parse_value=%u ", expr_parse_value);
    if (exp_value == expr_parse_value)
    {
      match_count++;
      Log("Match!!! match count = %d ", match_count);
    }
    else
    {
      Log("Did not Match!!! no_match_count = %d ", ++no_match_count);
    }
  }
  Log("expr_test");
  fclose(fp);
}

#include "./bnf/bnf.h"

void bnf_expr_test()
{

  int match_count = 0;
  int no_match_count = 0;
  char *NEMU_HOME = getenv("NEMU_HOME");
  char *filename = strcat(NEMU_HOME, "/tools/gen-expr/input.txt");
  printf("filename = %s\n", filename);
  FILE *fp = fopen(filename, "r");
  assert(fp != NULL);

  char *line = NULL;
  size_t len = 0;
  size_t nread = 0;
  while ((nread = getline(&line, &len, fp)) != -1)
  {
    /*  unsigned exp_value;
      char exp[nread];
      int tmp = sscanf(line, "%u %s", &exp_value, exp);
      assert(tmp == 2);*/

    char *token = strtok(line, " ");
    unsigned exp_value = atoi(token);
    char *exp = strtok(NULL, " ");
    // 移除换行符
    int str_size = strlen(exp);
    exp[str_size - 1] = '\0';

    Log("Retrieved test line content: res=%u,exp=%s", exp_value, exp);
    word_t expr_parse_value = calc(exp);
    Log("Expr_parse_value=%u ", expr_parse_value);
    if (exp_value == expr_parse_value)
    {
      match_count++;
      Log("Match!!! match count = %d ", match_count);
    }
    else
    {
      Log("Did not Match!!! no_match_count = %d ", ++no_match_count);
    }
  }
  Log("bnf_expr_test");
  fclose(fp);
}

void single_test()
{
  char *exp = "174+((((18*((((197))+(392)))+415))))*467-247/118";
  bool dd = true;
  int32_t expr_parse_value = expr(exp, &dd);
  Log("parse value = %u", expr_parse_value);
}