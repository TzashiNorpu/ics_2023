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
#include "local-include/reg.h"
#include <strings.h>

const char *regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void isa_reg_display()
{
  // 定义了 CONFIG_RVE 则返回 16，否则返回 32
  int reg_nums = MUXDEF(CONFIG_RVE, 16, 32);
  printf("[%8s] - [%10s]\n", "reg name", "reg  value");
  // 寄存器的位数：word_t -> typedef MUXDEF(CONFIG_ISA64, uint64_t, uint32_t) word_t;
  // riscv32 里是 32 位的
  for (int i = 0; i < reg_nums; i++)
  {
    printf("[%8s] - [%#10X]\n", reg_name(i), gpr(i));
  }
}
// 返回名字为s的寄存器的值, 并设置success指示是否成功
word_t isa_reg_str2val(const char *s, bool *success)
{
  int reg_nums = MUXDEF(CONFIG_RVE, 16, 32);
  for (int i = 0; i < reg_nums; ++i)
  {
    if (strncasecmp(s, regs[i], strlen(regs[i])) == 0)
    {
      *success = true;
      return gpr(i);
    }
  }
  // todo
  return -1;
}

void reg_info(const char *s, bool *exist, int *len)
{
  int reg_nums = MUXDEF(CONFIG_RVE, 16, 32);
  for (int i = 0; i < reg_nums; ++i)
  {
    // 跳过 $ 判断寄存器名称
    if (strncasecmp(s, regs[i], strlen(regs[i])) == 0)
    {
      *exist = true;
      *len = strlen(regs[i]);
      return;
    }
  }
}
