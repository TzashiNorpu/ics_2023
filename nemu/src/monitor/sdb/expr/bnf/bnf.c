
#include <stdio.h>
#include "error-output.h"
#include "expr.h"

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    error("wrong argc");
    return 1;
  }

  printf("res=%d\n", calc(argv[1]));
  return 0;
}