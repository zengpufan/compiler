输入为grammar.txt中的文法，以及input.txt中的一条token序列；

输出有多个：
1.处理后的文法 
2.消除左因子左递归后的文法 
3.first集合 
4.follow集合 
5.分析表（parsing_table.csv） 
6.分析结果的过程（result.csv）

功能包括：
1.消除左因子
2.消除左递归、复杂左递归
3.计算first、follow集合
4.判断是否为LL1文法
5.判断输入是否符合文法
