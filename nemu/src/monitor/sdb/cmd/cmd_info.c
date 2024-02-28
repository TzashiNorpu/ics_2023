#include "cmd_info.h"
#include "pattern.h"
#include <isa.h>
#include <debug.h>
#include "../watchpoint/watchpoint.h"
// 打印寄存器/监视器
int cmd_info(char *args)
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
    // 打印监视点信息
    print_watchlist_info();
    break;

  default:
    Log("Error command type...");
    break;
  }

  return 0;
}