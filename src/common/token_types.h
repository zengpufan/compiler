#pragma once
#include <list>
#include <string>
#include <iterator>
#include<iostream>
namespace compiler
{
    class token_type
    {

    private:
        // typename string [a-z]
        std::string _name;
        std::string _type;
        std::string _regex;
        int _id;

    public:
        token_type(std::string _name, std::string _type, std::string _regex, int _id)
            : _name(_name), _type(_type), _regex(_regex),_id(_id) {}
        std::string get_name()
        {
            return _name;
        }
        std::string get_type()
        {
            return _type;
        }
        std::string get_regex()
        {
            return _regex;
        }
        int get_id()
        {
            return _id;
        }
        bool operator==(token_type &other)
        {
            return this->_id == other.get_id();
        }
    };

    class token_types
    {

    private:
        std::list<token_type> ls;
        int cnt = 0;

    public:
        // token_types()
        // {
        //     ls.push_back(token_type("INVALID_TOKEN","string","",))
        // }
        void add(std::string _name, std::string _type, std::string _regex)
        {
            ls.push_back(token_type(_name, _type, _regex, cnt++));
        }
        std::list<token_type>::iterator begin()
        {
            return ls.begin();
        }
        std::list<token_type>::iterator end()
        {
            return ls.end();
        }
        std::string get_name(int id)
        {
            for (auto &it : ls)
            {
                if (it.get_id() == id)
                    return it.get_name();
            }
            return "";
        }
        static token_type INVALID()
        {
            return token_type("INVALID", "", "", -1);
        }
    };
}