#pragma once
#include <string>
#include "regular_expression.h"
namespace regular_expression
{
    class regular_expression_preprocessor
    {
    public:
        static std::string infix2postfix(const std::string &);
        static std::string prework(const std::string &);
        static inline bool is_symbol(const char &c) { return c > op_count ? true : false; }
    };
};