#pragma once
#include <set>
#include <list>
#include "config_reader.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
namespace compiler
{
    struct grammar
    {
        // public:
        std::string name;
        int id;
        std::list<std::vector<int>> exps;
        grammar(std::string name, int id)
            : name(name), id(id) {}
    };
    class grammar_list
    {
    public:
        static std::list<grammar> ls;
        static int cnt;
        static int start_id;

    public:
        // static const int INVALID = 0;
        // static std::list<std::pair<symbol, std::string>> token_list;
        // static void Init();
        // static std::string get_name_by_id(int id);
        // static symbol get_symbol_by_id(int id);
        static void add(std::string name)
        {
            ls.push_back(grammar(name, cnt++));
        }
        static int get_id_by_name(std::string name)
        {
            for (auto &gm : ls)
            {
                if (gm.name == name)
                    return gm.id;
            }
            return -1;
        }
        static std::string get_name_by_id(int id)
        {
            if (id == -1)
                return "epsilon";
            for (auto &gm : ls)
            {
                if (gm.id == id)
                    return gm.name;
            }
            return "null";
        }
        static bool is_terminal(int id)
        {
            for (auto &gm : ls)
            {
                if (gm.id == id)
                {
                    return gm.exps.size() == 0;
                }
            }
            return false;
        }
        static void init()
        {
        }
        static void print()
        {
            using namespace std;
            for (auto gm : ls)
            {
                cout << gm.name << "(" << grammar_list::get_id_by_name(gm.name) << ")"
                     << " : " << endl;
                int cnt = 0;
                for (auto exp : gm.exps)
                {
                    cout << ++cnt << ". ";
                    for (auto it : exp)
                    {
                        cout << "(" << it << ")" << grammar_list::get_name_by_id(it) << " & ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
        }
    };

}