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
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include "utils.h"

#include "isa.h"
#include <memory/vaddr.h>

static int is_batch_mode = false;
#define _1M 1000000

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  cpu_exec(-1);
  return 0;
}
extern CPU_state cpu;

// 打印寄存器/监视器
static int cmd_info(char *args)
{
  int result = info_pattern(args);
  // result == 0：满足校验
  if (result)
  {
    Log("Wrong info command pattern,please re-input.");
    return 0;
  }
  char type = ' ';
  sscanf(args, " %c ", &type);
  switch (type)
  {
  case 'r':
    isa_reg_display();
    break;

  case 'w':
    Log("info w...");
    break;

  default:
    Log("Error command type...");
    break;
  }

  return 0;
}

// 内存扫描
static int cmd_x(char *args)
{
  int result = x_pattern(args);
  // result == 0：满足校验
  if (result)
  {
    Log("Wrong x command pattern,please re-input.");
    return 0;
  }
  // printf("args =%s\n", args);
  if (strlen(args) >= 100)
  {
    Log("command args too long,please re-input.");
    return 0;
  }
  uint size = 0;
  word_t expr = 0;
  sscanf(args, " %i %i", &size, &expr);
  // printf("parse size=%i ,expr=%#X\n", size, expr);

  if (expr < CONFIG_MBASE || expr > CONFIG_MBASE + CONFIG_MSIZE)
  {
    Log("EXPR value exceed the range of MEM,please re-input.");
    return 0;
  }
  for (size_t i = 0; i < size; i++)
  {
    vaddr_t addr = expr + 4 * i;
    printf("%0#10x  :   %0#10x\n", addr, vaddr_read(addr, 4));
  }

  return 0;
}

static int cmd_si(char *args)
{
  uint steps = 1;
  if (args)
  {
    int result = si_pattern(args);
    // result == 0：满足校验
    // Assert(result == 0, "Wrong si command pattern");
    if (result)
    {
      Log("Wrong si command pattern,please re-input.");
      return 0;
    }

    // i:如果输入字符串以“0x”或“0X”开始，则为十六进制；如果字符串以“0”开始，则为八进制；其余情况为十进制
    sscanf(args, " %10i ", &steps);
  }
  // 太大的步数debug期间没有意义
  if (steps <= 0 || steps > _1M)
  {
    Log("Exceed range of execuet steps,please re-input.");
    return 0;
  }
  cpu_exec(steps);
  return 0;
}

static int cmd_q(char *args)
{
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);

static struct
{
  const char *name;
  const char *description;
  int (*handler)(char *);
} cmd_table[] = {
    {"help", "Display information about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},

    /* TODO: Add more commands */
    {"si", "si [N]:Execute N steps,default 1 step", cmd_si},
    {"info", "info r/w:Display information about register/watchpoint", cmd_info},
    {"x", "x N EXPR:Display N bytes information of EXPR address【MEM】", cmd_x},

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL)
  {
    /* no argument given */
    for (i = 0; i < NR_CMD; i++)
    {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else
  {
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(arg, cmd_table[i].name) == 0)
      {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode()
{
  is_batch_mode = true;
}

void sdb_mainloop()
{
  if (is_batch_mode)
  {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL;)
  {
    // str 里是包含空格的字符串
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    // cmd：第一个空格以前的部分，作为命令处理
    char *cmd = strtok(str, " ");
    if (cmd == NULL)
    {
      continue;
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    // args：第一个空格以后的部分，作为参数处理
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end)
    {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0)
      {
        if (cmd_table[i].handler(args) < 0)
        {
          return;
        }
        break;
      }
    }

    if (i == NR_CMD)
    {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}

void init_sdb()
{
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
