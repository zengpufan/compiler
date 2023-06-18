#ifndef _ANALYZE_H_
#define _ANALYZE_H_

/* buildSymtab用前序遍历对AST构建符号表 */
void buildSymtab(TreeNode *);

/* typeCheck performs用后序遍历对AST进行类型检查 */
void typeCheck(TreeNode *);

#endif
