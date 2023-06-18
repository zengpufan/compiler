#include "globals.h"

/* 将 NO_PARSE 设为 TRUE 可以只完成词法分析 */
#define NO_PARSE FALSE
/* 将 NO_ANALYZE 设为 TRUE 可以只完成词法和语法分析 */
#define NO_ANALYZE FALSE

/* 将 NO_CODE to TRUE 可以不生成中间代码 */
#define NO_CODE FALSE

// scan在嵌套结构上比较特殊，被parse所调用
#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "cgen.h"
#endif
#endif
#endif

// 下面这些变量中都在globals.h中被extern公开，作为全局变量被其他文件使用

int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

// 跟踪标志符

int EchoSource = TRUE; // 跟踪信息由scan打印
int TraceScan = TRUE;  // 跟踪信息由scan打印
int TraceParse = TRUE; // 由main打印
int TraceAnalyze = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

main(int argc, char *argv[])
{
  TreeNode *syntaxTree;
  char pgm[120]; /* source code file name */
  if (argc != 2) // 检查参数个数
  {
    fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    exit(1);
  }
  strcpy(pgm, argv[1]);
  if (strchr(pgm, '.') == NULL) // 自动检测补全后缀
    strcat(pgm, ".tny");
  source = fopen(pgm, "r");
  if (source == NULL)
  {
    fprintf(stderr, "File %s not found\n", pgm);
    exit(1);
  }

  // 输出导至标准输出
  listing = stdout; /* send listing to screen */
  fprintf(listing, "\nTINY COMPILATION: %s\n", pgm);
#if NO_PARSE
  while (getToken() != ENDFILE)
    ; // 关键函数1
#else
  // parse()自己调用了getToken()
  syntaxTree = parse(); // 关键函数2
  if (TraceParse)
  {
    fprintf(listing, "\nSyntax tree:\n");
    printTree(syntaxTree);
  }
#if !NO_ANALYZE
  if (!Error) // Error是一个全局变量，承接parse()输出的ERROR状态
  {
    if (TraceAnalyze)
      fprintf(listing, "\nBuilding Symbol Table...\n");
    buildSymtab(syntaxTree); // 关键函数3
    if (TraceAnalyze)
      fprintf(listing, "\nChecking Types...\n");
    typeCheck(syntaxTree); // 关键函数4
    if (TraceAnalyze)
      fprintf(listing, "\nType Checking Finished\n");
  }           // 上面这两个函数可以看成一个整体，输入是一棵语法树，输出是一颗带标记的语法树，和一张符号表
#if !NO_CODE
  if (!Error) // 又是一次错误检查
  {
    char *codefile;
    // 组建输出文件的文件名
    int fnlen = strcspn(pgm, ".");
    codefile = (char *)calloc(fnlen + 4, sizeof(char));
    strncpy(codefile, pgm, fnlen);
    strcat(codefile, ".tm");
    code = fopen(codefile, "w");
    if (code == NULL)
    {
      printf("Unable to open %s\n", codefile);
      exit(1);
    }
    codeGen(syntaxTree, codefile); // 关键函数5
    fclose(code);
  }
#endif
#endif
#endif
  fclose(source);
  return 0;
}
