/*
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-05-01 20:41:09
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-09 21:35:26
 * @FilePath: \grammar-analysis\src\compiler\compiler.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "compiler.h"
#include "config_reader.h"
#include <iostream>
#include "lexical_analysis.h"
#include "token_types.h"
#include "LL1_prework.h"
#include <string>
#include <fstream>
#include <sstream>
#include "First.h"
#include "Follow.h"
#include "analyse_table.h"
#include "sytax_tree.h"
#include "expection.h"
#include "ErrorMessage.h"
namespace compiler
{
    void myCompiler::init_token_types()
    {
        token_reader reader;
        std::string src = "../src/config/token.txt";
        if (reader.open(src) == false)
        {
            throw Expection("token_reader", "can't open file " + src);
        }
        this->_token_types = reader.read();
    }
    void myCompiler::read_code(const std::string &src)
    {
        std::ifstream ifs;
        ifs.open(src);
        if (ifs.is_open() == false)
        {
            std::cout << "can't open src code" << std::endl;
            return;
        }
        std::string line;
        while (std::getline(ifs, line))
        {
            code.push_back(line);
        }
        return;
    }
    void myCompiler::init_grammar_list()
    {
        grammar_reader reader;
        if (reader.open("../src/config/grammar.txt") == false)
        {
            std::cout << "open config failed" << std::endl;
        }
        grammar_list::ls.push_back(grammar("Epsilon", -1));
        grammar_list::ls.push_back(grammar("File_end", -2));
        reader.read();
        // grammar_list::print();
    }
    myCompiler::myCompiler(const std::string &src)
    {
        try
        {
            // read token types from config
            init_token_types();
            std::cout << std::endl
                      << "read token types finished" << std::endl
                      << std::endl;

            // read code from source file
            read_code(src);
            std::cout << "read code finished" << std::endl;

            // create lexcal analyse class
            lexical_analysis le(_token_types);

            // analyse code
            // std::list<token> ls;
            std::list<token> token_list = le.Run(code);
            // for (auto &line : code)
            // {
            //     std::istringstream iss(line.second);
            //     std::string s;
            //     while (iss >> s)
            //     {
            //         std::list<token> l = le.Run(s);
            //         ls.splice(ls.end(), l);
            //     }
            // }
            std::cout << "tokenList generated" << std::endl;

            // print the tokenList
            std::cout << "---------------------token list------------------------" << std::endl;
            for (auto &it : token_list)
            {
                std::cout << "< " << it.get_value() << " , " << _token_types->get_name(it.get_id()) << " " << it.get_id() << " >" << std::endl;
            }
            std::cout << "---------------------token list------------------------" << std::endl;

            // read terminal symbols from token config
            for (auto token : *_token_types)
            {
                grammar_list::add(token.get_name());
            }
            // read non-terminal symbols from grammar config
            init_grammar_list();

            // print the grammar list
            std::cout << std::endl
                      << "!!grammarList generated!!" << std::endl
                      << std::endl;

            std::cout << "---------------------grammer list------------------------" << std::endl;
            grammar_list::print();
            std::cout << "---------------------grammer list------------------------" << std::endl;

            // LL1
            remove_left_recursion();
            First first = First();
            first.print();
            Follow follow = Follow(first);
            follow.print();
            std::cout << "analyse_table generated" << std::endl;
            analyse_table tb = analyse_table(first, follow);

            std::cout << "---------------------LL1 analyse table------------------------" << std::endl;
            tb.print();
            std::cout << "---------------------LL1 analyse table------------------------" << std::endl;

            sytax_tree tree = sytax_tree(tb, token_list);

            std::cout << "!! LL(1) analyse finished" << std::endl;

            std::cout << "---------------------sytax tree------------------------" << std::endl;
            tree.print();
            std::cout << "---------------------sytax tree------------------------" << std::endl;
            std::cout<<grammar_list::get_name_by_id(grammar_list::start_id)<<"\n";
        }
        catch (Expection &e)
        {
            std::cout << "compiler error, compilation can't start" << std::endl;
            e.print();
        }
        ErrorMessage::printErrorMessage();
    }

}
