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

#include "watchpoint.h"
#include "../expr/bnf/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
// head用于组织使用中的监视点结构, free_用于组织空闲的监视点结构
void init_wp_pool()
{
  int i;
  for (i = 0; i < NR_WP; i++)
  {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}
// 申请新节点
WP *new_wp()
{
  if (!free_)
    Assert(0, "没有可用节点了");
  // 拿掉 free_ 的头节点
  WP *free_next = free_->next;
  WP *free_head = free_;
  free_ = free_next;
  // 把申请到的节点挂载到 head 的头节点
  free_head->next = head;
  head = free_head;
  return free_head;
}
// 归还节点
void free_wp(WP *wp)
{
  Assert(wp != NULL, "Can't release NULL node");
  WP *temp = head;
  WP *prev = NULL;
  WP *next = NULL;
  // 找到归还节点的上一节点和下一节点
  // 归还的如果是首节点
  if (wp == temp)
  {
    head = head->next;
  }
  // 归还的如果不是首节点
  else
  {
    while (temp && temp->next)
    {
      if (temp->next == wp)
      {
        prev = temp;
        next = temp->next->next;
        break;
      }
      temp = temp->next;
    }
    // 找不到节点报错
    Assert(temp->next == wp, "Can't find index:%d WP point\n", wp->NO);
    // 从 head 中拿掉这个节点
    prev->next = next;
  }
  // 放回空闲链表的头节点中
  if (!free_)
    wp->next = NULL;
  else
    wp->next = free_;
  free_ = wp;
}

WP *get_wp_by_index(int N)
{
  WP *p = head;
  while (p)
  {
    if (p->NO == N)
      return p;
    p = p->next;
  }
  return NULL;
}

void print_watchlist_info()
{
  WP *p = head;
  printf("used points:\n");
  while (p)
  {
    printf("index=%d,expression=%s,value=%lu\n", p->NO, p->expr, p->val);
    p = p->next;
  }

  p = free_;
  printf("free points:\n");
  while (p)
  {
    printf("index=%d\n", p->NO);
    p = p->next;
  }
}

void revaluation_watchpoint_list(WP *wp)
{
  WP *p = head;
  while (p)
  {
    long new_val = calc(p->expr);
    if (new_val != p->val)
    {
      p->val = new_val;
      wp = p;
      return;
    }
    p = p->next;
  }
  wp = NULL;
}