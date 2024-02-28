#pragma once
#include "../sdb.h"
#include <debug.h>

typedef struct watchpoint
{
    int NO;
    struct watchpoint *next;

    /* TODO: Add more members if necessary */
    char *expr;
    long val;
} WP;

WP *new_wp();
WP *get_wp_by_index(int N);
void free_wp(WP *wp);
void print_watchlist_info();
void revaluation_watchpoint_list(WP *wp);