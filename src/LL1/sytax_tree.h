#pragma once
#include <list>
#include <string>
#include "analyse_table.h"
#include "token.h"
#include "token_types.h"
namespace compiler
{
    class sytax_node
    {
    public:
        int id;
        std::string value;
        std::list<sytax_node *> ne;
        sytax_node(int id, std::string value);
    };
    class sytax_tree
    {
    public:
        sytax_node *head;
        sytax_tree(analyse_table &tb, std::list<token> &token_list);
        void print();
        ~sytax_tree();
    };
}