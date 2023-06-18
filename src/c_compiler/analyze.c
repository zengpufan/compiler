#include "globals.h"
#include "symtab.h"
#include "analyze.h"

/* 内存位置的计数器 */
static int location = 0;

/* Procedure traverse遍历t指向的AST:
 * 对preProc前序遍历
 * 对postProc后序遍历
 */
static void traverse(TreeNode *t,
                     void (*preProc)(TreeNode *),  // 这个函数用于先根遍历
                     void (*postProc)(TreeNode *)) // 这个函数用于后根遍历
{
  if (t != NULL)
  {
    preProc(t);
    {
      int i;
      for (i = 0; i < MAXCHILDREN; i++)
        traverse(t->child[i], preProc, postProc);
    }
    postProc(t);
    traverse(t->sibling, preProc, postProc);
  }
}

// 这个函数无所事事，两次用来填充空位
// buildSymtab()用的是先根遍历
// typeCheck()用的是后根遍历
// 巧妙的设计
static void nullProc(TreeNode *t)
{
  if (t == NULL)
    return;
  else
    return;
}

/* insertNode inserts将t指向的语法树中的标识符插入符号表 */
static void insertNode(TreeNode *t)
{
  switch (t->nodekind)
  {
  case StmtK: // 五种语句中，只有两种要插入符号表
    switch (t->kind.stmt)
    {
    case AssignK:
    case ReadK: // 先查询，后插入
      if (st_lookup(t->attr.name) == -1)
        /* 如果不在内存中，就按新的变量对待 */
        st_insert(t->attr.name, t->lineno, location++);
      else
        /* 如果已经在表中, 只使用行号 */
        st_insert(t->attr.name, t->lineno, 0);
      break;
    default:
      break;
    }
    break;
  case ExpK: // 表达式中只有一种要插入符号表
    switch (t->kind.exp)
    {
    case IdK:
      if (st_lookup(t->attr.name) == -1)
        st_insert(t->attr.name, t->lineno, location++);
      else
        st_insert(t->attr.name, t->lineno, 0);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

/* buildSymtab 通过对语法树的前序遍历构建符号表 */
void buildSymtab(TreeNode *syntaxTree)
{
  traverse(syntaxTree, insertNode, nullProc); // 传入两个函数指针
  if (TraceAnalyze)
  {
    fprintf(listing, "\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

static void typeError(TreeNode *t, char *message)
{
  fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
  Error = TRUE; // 全局变量
}

/* checkNode 对单个树节点进行类型检查 */
// 两个任务：标记类型/检查类型
static void checkNode(TreeNode *t)
{
  switch (t->nodekind)
  {
  case ExpK:
    switch (t->kind.exp)
    {
    case OpK: // 表达式根据符号来检查类型
      if ((t->child[0]->type != Integer) ||
          (t->child[1]->type != Integer))
        typeError(t, "Op applied to non-integer"); // 错误处理1
      if ((t->attr.op == EQ) || (t->attr.op == LT))
        t->type = Boolean; // 标记类型
      else
        t->type = Integer; // 标记类型
      break;
    case ConstK:
    case IdK:
      t->type = Integer; // 变量/常量一定是Integer
      break;
    default:
      break;
    }
    break;
  case StmtK: // 语句是没有type的，但是语句中的表达式有type，两种要求Boolean，两种要求Integer
    switch (t->kind.stmt)
    {
    case IfK:
      if (t->child[0]->type == Integer)
        typeError(t->child[0], "if test is not Boolean"); // 错误处理2～5
      break;
    case AssignK:
      if (t->child[0]->type != Integer)
        typeError(t->child[0], "assignment of non-integer value");
      break;
    case WriteK:
      if (t->child[0]->type != Integer)
        typeError(t->child[0], "write of non-integer value");
      break;
    case RepeatK:
      if (t->child[1]->type == Integer)
        typeError(t->child[1], "repeat test is not Boolean");
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

/* typeCheck对AST进行后序遍历以进行类型检查 */
void typeCheck(TreeNode *syntaxTree)
{
  traverse(syntaxTree, nullProc, checkNode); // 传入两个函数指针
}
