// 这里生命的是全局变量，包括文件指针，行号，抽象语法树的结构，以及一些标识符
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// 保留字的大小
#define MAXRESERVED 8

// 枚举标识token的类型
typedef enum
{
   // 编译器运行需要的两个状态
   ENDFILE,
   ERROR,
   /* reserved words */
   // 保留字8个
   IF,
   THEN,
   ELSE,
   END,
   REPEAT,
   UNTIL,
   READ,
   WRITE,
   // 标识符2种
   ID,
   NUM,
   // 特殊符号10个
   ASSIGN,
   EQ,
   LT,
   PLUS,
   MINUS,
   TIMES,
   OVER,
   LPAREN,
   RPAREN,
   SEMI
} TokenType;

extern FILE *source;  // 源文件
extern FILE *listing; // 输出
extern FILE *code;    // 中间代码文件

extern int lineno; // 行号

// 抽象语法树的结构，见于课本p135

// 语法树的节点类型
typedef enum
{
   StmtK,
   ExpK
} NodeKind;

// 语句类型，即statement类型
typedef enum
{
   IfK,
   RepeatK,
   AssignK,
   ReadK,
   WriteK
} StmtKind;

// 表达式类型，即expression类型
typedef enum
{
   OpK,
   ConstK,
   IdK
} ExpKind;

// ExpType 被用来进行类型检查
typedef enum
{
   Void,
   Integer,
   Boolean
} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
   struct treeNode *child[MAXCHILDREN];
   struct treeNode *sibling;
   int lineno;
   NodeKind nodekind;
   union
   {
      StmtKind stmt;
      ExpKind exp;
   } kind;
   union
   {
      TokenType op;
      int val;
      char *name;
   } attr;
   ExpType type; // 对exps进行类型检查
} TreeNode;

// 用于追踪的标识符flags

/* EchoSource = TRUE 使源程序被带着行号在显示在列表文件中 */
extern int EchoSource;

/* TraceScan = TRUE 使每个token的信息被打印到listing中 */
extern int TraceScan;

/* TraceParse = TRUE 使语法树的结构被打印 */
extern int TraceParse;

/* TraceAnalyze = TRUE 使符号表的插入和查询会被反馈到listing中 */
extern int TraceAnalyze;

/* TraceCode = TRUE 使注释会被打印到TM文件中 */
extern int TraceCode;

/* Error = TRUE 会阻止进一步的pass */
extern int Error;
#endif
