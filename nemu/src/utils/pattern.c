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

#include <utils.h>
#include <regex.h>

int si_pattern(char *p)
{
  regex_t regex;
  // 正则表达式:
  const char *pattern = "^[ ]*(0x|0X|0)?[0-9]*[ ]*$";

  // 编译正则表达式
  int result = regcomp(&regex, pattern, REG_EXTENDED);
  if (result != 0)
  {
    printf("Error compiling regex\n");
    return 1;
  }

  // 进行模式匹配
  result = regexec(&regex, p, 0, NULL, 0);
  // result = 0 -> 满足格式

  // 释放正则表达式
  regfree(&regex);

  return result;
}
