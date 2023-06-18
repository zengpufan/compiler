#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* st_insert 向表中插入行号和在内存中的位置
 * loc = memory location只在第一次运行时生效
 */
void st_insert(char *name, int lineno, int loc);

/* st_lookup 返回变量在内存(memory)中的位置否则返回-1 */
int st_lookup(char *name);

/* printSymTab打印符号表到listing文件 */
void printSymTab(FILE *listing);

#endif
