/*
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-04-17 13:24:25
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-08 12:42:06
 * @FilePath: \grammar-analysis\src\common\expection.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include <string>
#include <iostream>
namespace compiler
{

    class Expection
    {
    private:
        std::string _file;
        std::string _description;

    public:
        Expection(const std::string &_file, const std::string &_description);
        void print();
    };
}