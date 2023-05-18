/*
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-04-16 17:15:06
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-08 22:59:06
 * @FilePath: \grammar-analysis\src\common\config_reader.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
/*
read token.txt and output symbol(class)
read grammar.txt and output grammar
*/
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include "token_types.h"
#include "grammar_list.h"
namespace compiler
{
    class token_reader
    {
    private:
        std::ifstream ifs;

    public:
        bool open(std::string s)
        {
            ifs.open(s);
            return ifs.is_open();
        }
        token_types *read()
        {

            token_types *res = new token_types;

            std::string name, regex, tp;
            while (ifs >> name, ifs >> tp, ifs >> regex)
            {
                res->add(name, tp, regex);
            }
            return res;
        }
        // std::list<std::pair<symbol,std::string>>read()
        // {
        //     std::list<std::pair<symbol,std::string>>res;

        //     std::string name,regex,tp;
        //     while(ifs>>name,ifs>>tp,ifs>>regex)
        //     {
        //         res.push_back({symbol(true,name),regex});
        //     }
        //     std::cout<<"read finished"<<"\n";
        //     return res;
        // }
    };
    class grammar_reader
    {
    private:
        std::ifstream ifs;

    public:
        bool open(std::string s)
        {
            ifs.open(s);
            return ifs.is_open();
        }
        void read();
    };
}