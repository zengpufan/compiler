#pragma once
#include "token_types.h"
#include "config_reader.h"
#include "lexical_analysis.h"
#include <string>
namespace compiler
{
    class myCompiler
    {
    private:
        token_types *_token_types;
        std::list<std::string>code;
    public:
        void read_code(const std::string&);
        void init_token_types();
        void init_grammar_list();
        myCompiler(const std::string &src);
        
    };

}