#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN token的最大长度 */
#define MAXTOKENLEN 40

// 两条返回路径，token的类型通过函数返回值告知
// token的值通过全局变量传递（只有id/num两种类型要用到）

/* tokenString array 存储每个token的词素 */
// 变量声明，将这个全局变量的作用域扩展到其他文件
extern char tokenString[MAXTOKENLEN + 1];

/* getToken的接口返回源文件中的下一个token，一次只返回一个token */
TokenType getToken(void);

#endif
