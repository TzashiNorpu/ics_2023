#include "cmd_si.h"
int cmd_si(char *args)
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