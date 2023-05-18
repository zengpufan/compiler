#pragma once
#include <iostream>
#include "location.h"
namespace compiler
{

    class token
    {
    private:
        int token_id;
        std::string token_value;

    public:
        location _location;

    public:
        token(int id, std::string token_value, location _location)
            : token_id(id), token_value(token_value),_location(_location) {}
        void set_value(std::string value)
        {
            this->token_value = value;
        }
        std::string get_value()
        {
            return this->token_value;
        }
        int get_id()
        {
            return this->token_id;
        }
    };
}