#include "cmd_x.h"
// 内存扫描
int cmd_x(char *args)
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