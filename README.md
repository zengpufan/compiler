<!--
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-05-18 10:40:36
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-18 12:30:12
 * @FilePath: \undefinedc:\Users\zpf\Desktop\cp\compiler\README.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
# compiler
## 项目结构
common文件夹是几个模块的公共部分
compiler调用几个模块，运行编译器
lexical_analysis是词法分析
regex是正则表达式
LL1是语法分析LL1（目前没有实现，只有语法树的数据结构，如果不使用LL1分析，就改成别的名字，但语法分析树的结构不要改）
config是一些配置文件，存储词法的正则表达式，语法表达式。
## common文件夹的详细介绍
ErrorMessage用来报错，这是一个static类，使用时直接调用addErrorMessage方法即可，最后整个项目会统一报错。  
expection负责抛出异常，如果出现不能解决的错误导致编译不得不终止，throw一个expection类即可  
token_types记录token的类型，这个类型由config_reader从配置文件中读取
grammar_list记录语法表达式，这些语法表达式由config_reader从配置文件中读取
（词法分析中生成的符号都看作终结符号，语法分析中生成的符号看作非终结符号，每个符号用一个对应的id表示）
## 各模块的任务

词法分析输出一个token序列。  
语法分析根据词法分析的token序列生成语法分析树（调用grammar_list读取对应的文法）（生成树的数据结构在LL1文件夹中，可以不使用LL1分析）。
语义分析根据语法分析的生成树进行语义分析    

大家如果对项目流程不清楚就看看compiler.cpp中的构造函数，大致的运行流程写的很清楚，我基本上每局都注释了。

## 代码规范
变量命名格式不要求，每个人的子模块自建一个文件夹，非必要不要修改其他文件夹，以防冲突。
