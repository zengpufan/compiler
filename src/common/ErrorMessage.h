#pragma once
#include <stack>
#include <string>
#include <iostream>
#include "location.h"
namespace compiler
{
    class ErrorMessage
    {
    private:
        static std::stack<std::pair<std::string, location>> ErrorMessageStack;

    public:
        static void addErrorMessage(std::string s, location l)
        {
            ErrorMessageStack.push(std::make_pair(s, l));
        }
        static void printErrorMessage()
        {
            using namespace std;
            while (!ErrorMessageStack.empty())
            {
                auto tp = ErrorMessageStack.top();
                ErrorMessageStack.pop();
                cout << "(" << tp.second.get_row() << "," << tp.second.get_col() << ") " << tp.first << endl;
            }
        }
    };

}