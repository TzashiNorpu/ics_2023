#include "cmd_d.h"
#include "../watchpoint/watchpoint.h"
int cmd_d(char *p)
{
  int index = -1;
  sscanf(p, " %10i ", &index);
  WP *point = get_wp_by_index(index);
  Assert(point, "Can not find %d index watchpoint", index);
  free_wp(point);
  return 0;
}