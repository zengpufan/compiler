#include <iostream>
#include "regular_expression_preprocessor.h"
#include "NFA.h"
#include "algrithm.h"
#include "regular_expression_engine.h"
#include <fstream>
#include "lexical_analysis.h"
#include"expection.h"
#include"compiler.h"
using namespace compiler;
using namespace std;
using namespace regular_expression;
string test_string1 = "(a|b)*";
string test_string2 = "(a|b)*&a&(a|b)";
string test_string3 = "(a|b)*&a&(a|b)&(a|b)&(a|b)";
string test_string4 = "a*(abb)*b(b|a)*"; // homework
string test_string5 = "a*(abb)*b(b|a)*\\*[a-b]";
string test_string6 = "[0-9]*.\\[([0-6]|[8-9])*";
string test_string7 = "\"([a-z])*\"";
// void test_lexical_analysis(char *s)
// {
//     compiler::lexical_analysis cp;
//     // code.txt是要分析文件的名称
//     try
//     {
//         cp.Run_file(s);
//         /* code */
//     }
//     catch (compiler::Expection &e)
//     {
//         e.log();
//     }
// }
// void show_dfa_nfa(const string &s)
// {
//     _show(s);
// }

int main(int argc, char **argv)
{

    //cout<<argv[1]<<endl;
    compiler::myCompiler cp(argv[1]);
    //symbol_list::Init();
    if (argc == 1)
    {
        cout << "too few args\n";
        return 0;
    }
    //test_lexical_analysis(argv[1]);
    // show_dfa_nfa(test_string4);

    //system("pause");
}