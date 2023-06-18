#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// 实现哈希+链表的插入+查询功能

/* hash表的大小 */
#define SIZE 211

/* SHIFT在hash函数中作为乘法器 */
#define SHIFT 4

static int hash(char *key)
{
  int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  {
    temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* 变量被引用的源代码中的行号的list */
typedef struct LineListRec
{
  int lineno;
  struct LineListRec *next;
} *LineList;

/* 每个变量的记录表
 * 包括 name,
 * assigned memory location,
 * Linelist
 */
typedef struct BucketListRec
{
  char *name;
  LineList lines;
  int memloc; /* 变量的memory location */
  struct BucketListRec *next;
} *BucketList;

// 哈希表是作为全局变量存储，没有extern
// 但是代码生成模块会用到lookup函数
// 所以实际上是共享的
static BucketList hashTable[SIZE];

/* st_insert 向表中插入行号和在内存中的位置
 * loc = memory location只在第一次运行时生效
 */
void st_insert(char *name, int lineno, int loc)
{
  int h = hash(name);
  BucketList l = hashTable[h];                        // 从哈希表的某一项拿出链表
  while ((l != NULL) && (strcmp(name, l->name) != 0)) // 在哈希表项链表里找某个符号的链表
    l = l->next;
  if (l == NULL) /* 变量不在表中 */
  {
    l = (BucketList)malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList)malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc; // 记录下位置号
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l;
  }
  else /* 原来在表中有，只加入行号 */
  {
    LineList t = l->lines;
    while (t->next != NULL)
      t = t->next; // 走到链表最末端
    t->next = (LineList)malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
}

/* st_lookup 返回变量在内存(memory)中的位置否则返回-1 */
int st_lookup(char *name)
{
  int h = hash(name);
  BucketList l = hashTable[h];                        // 从哈希表的某一项拿出链表
  while ((l != NULL) && (strcmp(name, l->name) != 0)) // 在哈希表项链表里找某个符号的链表
    l = l->next;
  if (l == NULL)
    return -1;
  else
    return l->memloc; // 返回的是位置号
}

/* printSymTab打印符号表到listing文件 */
void printSymTab(FILE *listing)
{
  int i;
  fprintf(listing, "Variable Name  Location   Line Numbers\n");
  fprintf(listing, "-------------  --------   ------------\n");
  for (i = 0; i < SIZE; ++i)
  {
    if (hashTable[i] != NULL)
    {
      BucketList l = hashTable[i];
      while (l != NULL)
      {
        LineList t = l->lines;
        fprintf(listing, "%-14s ", l->name);
        fprintf(listing, "%-8d  ", l->memloc);
        while (t != NULL)
        {
          fprintf(listing, "%4d ", t->lineno);
          t = t->next;
        }
        fprintf(listing, "\n");
        l = l->next;
      }
    }
  }
}
