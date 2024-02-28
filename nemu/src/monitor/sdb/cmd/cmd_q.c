#include "cmd_q.h"
#include <utils.h>
int cmd_q(char *args)
{
  nemu_state.state = NEMU_QUIT;
  return -1;
}