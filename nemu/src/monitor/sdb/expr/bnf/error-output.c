#include "error-output.h"
#include <stdio.h>
#include <stdlib.h>
extern char *user_input;

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
void errorTok(BNFToken *tok, char *fmt, ...)
{
  va_list VA;
  va_start(VA, fmt);
  verrorAt(tok->loc, fmt, VA);
  exit(1);
}
