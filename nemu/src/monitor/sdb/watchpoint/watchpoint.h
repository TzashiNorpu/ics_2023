#pragma once
#include "../sdb.h"
#include <debug.h>

typedef struct watchpoint
{
    int NO;
    struct watchpoint *next;

    /* TODO: Add more members if necessary */

} WP;

WP *new_wp();
void free_wp(WP *wp);