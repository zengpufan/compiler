#include "regular_expression_preprocessor.h"
#include <string>
#include <stack>
#include <iostream>
#include "expection.h"
namespace regular_expression
{
    typedef regular_expression_preprocessor preprocessor;
    using namespace compiler;
    std::string preprocessor::prework(const std::string &input)
    {
        // check valid
        if (input.size() == 0)
            throw Expection("Location::regex", "invalid input:input can't be empty");
        // std::string s = input;
        auto remove_blank = [&](const std::string &s) -> std::string
        {
            std::string res;
            for (int i = 0; i < s.size(); i++)
                if (s[i] != ' ')
                    res.push_back(s[i]);
            return res;
        };
        auto translate_range = [&](const std::string &s) -> std::string
        {
            auto valid = [&](const char &a, const char &b) -> bool
            {
                if (
                    (
                        ((a >= 'a' && a <= 'z') && (b >= 'a' && b <= 'z')) ||
                        ((a >= 'A' && a <= 'Z') && (b >= 'A' && b <= 'Z')) ||
                        ((a >= '0' && a <= '9') && (b >= '0' && b <= '9'))))
                {
                    if (a > b)
                        return false;
                    else
                        return true;
                }
                else
                    return false;
            };
            std::string res;
            for (int i = 0; i < s.size(); i++)
            {
                if (s[i] != op::Lsquare)
                    res.push_back(s[i]);
                else
                {
                    if (i + 4 >= s.size() || s[i + 4] != op::Rsquare || s[i + 2] != '-' || !valid(s[i + 1], s[i + 3]))
                        throw Expection("Location::regex", "invalid input: [] don't match");
                    char a = s[i + 1], b = s[i + 3];
                    res.push_back(op::Left);
                    for (char c = a; c <= b; c++)
                    {
                        if (c != a)
                            res.push_back(op::Or);
                        res.push_back(c);
                    }
                    res.push_back(op::Right);
                    i += 4;
                }
            }
            return res;
        };
        auto ESC = [&](const std::string &s) -> std::string
        {
            std::string res;
            for (int i = 0; i < s.size(); i++)
            {
                if (s[i] == '\\')
                {

                    if (i + 1 == s.size())
                    {
                        throw Expection("Location::regex", "invalid input: /");
                    }
                    else
                    {
                        i++;
                        res.push_back(s[i]);
                    }
                    // continue;
                }
                else
                {
                    switch (s[i])
                    {
                    case '(':
                        res.push_back(op::Left);
                        break;
                    case ')':
                        res.push_back(op::Right);
                        break;
                    case '*':
                        res.push_back(op::closure);
                        break;
                    case '&':
                        res.push_back(op::And);
                        break;
                    case '|':
                        res.push_back(op::Or);
                        break;
                    case '[':
                        res.push_back(op::Lsquare);
                        break;
                    case ']':
                        res.push_back(op::Rsquare);
                        break;
                    default:
                        res.push_back(s[i]);
                        break;
                    }
                }
            }
            return res;
        };
        auto add_and = [&](const std::string &s) -> std::string
        {
            std::string res;
            for (int i = 0; i < s.size(); i++)
            {
                res.push_back(s[i]);
                if (i + 1 == s.size())
                    break;
                if (
                    (is_symbol(s[i]) && (is_symbol(s[i + 1]) || s[i + 1] == op::Left)) ||
                    (is_symbol(s[i + 1]) && (s[i] == op::closure || s[i] == op::Right)) ||
                    (s[i + 1] == op::Left && (s[i] == op::Right || s[i] == op::closure)))
                {
                    res.push_back(op::And);
                }
            }
            return res;
        };
        std::string s = remove_blank(input);
        s = ESC(s);
        s = translate_range(s);
        s = add_and(s);
        // for (int i = 0;i < s.size();i++)
        //{
        //     if (is_symbol(s[i]))std::cout << s[i];
        //     else std::cout << "?";
        // }
        return s;
    }

    std::string preprocessor::infix2postfix(const std::string &s)
    {
        std::stack<char> op_stack;
        std::stack<std::string> symbol_stack;
        int priorityl[20] = {}, priorityr[20] = {};

        {
            //()*|&
            priorityl[op::Left] = 1;
            priorityr[op::Left] = 5;
            priorityl[op::closure] = 4;
            priorityr[op::closure] = 4;
            priorityl[op::And] = 3;
            priorityr[op::And] = 3;
            priorityl[op::Or] = 2;
            priorityr[op::Or] = 2;
            priorityl[op::Right] = 5;
            priorityr[op::Right] = 1;
        }

        // a is priority 1;b is priority 0
        auto compare_priority = [&](const char &a, const char &b)
        {
            return priorityl[a] >= priorityr[b];
        };

        auto cal = [&](const char &c)
        {
            if (c == op::And || c == op::Or)
            {
                if (symbol_stack.size() < 2)
                    throw Expection("Location::regex", "invalid expression: operator &| don't match");
                std::string x = symbol_stack.top();
                symbol_stack.pop();
                std::string y = symbol_stack.top();
                symbol_stack.pop();
                // std::cout << "xy" << " " << x << " " << y << "\n";
                y += x;
                y.push_back(c);
                symbol_stack.push(y);
            }
            else if (c == op::closure)
            {
                std::string x = symbol_stack.top();
                symbol_stack.pop();
                x.push_back(c);
                symbol_stack.push(x);
            }
        };

        for (int i = 0; i < s.size(); i++)
        {
            if (is_symbol(s[i]))
            {
                symbol_stack.push(s.substr(i, 1));
            }
            else
            {
                while (!op_stack.empty())
                {
                    if (compare_priority(op_stack.top(), s[i]))
                    {
                        if (op_stack.top() == op::Left && s[i] == op::Right)
                            break;
                        cal(op_stack.top());
                        op_stack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                if (s[i] == op::Right)
                {

                    if (op_stack.empty() || op_stack.top() != op::Left)
                        throw Expection("Location::regex", "invalid input: () don't match");
                    else
                        op_stack.pop();
                }
                else
                {
                    op_stack.push(s[i]);
                }
            }
        }
        while (!op_stack.empty())
        {
            char c = op_stack.top();
            if (c == op::Left)
                throw Expection("Location::regex", "invalid input: () don't match");
            else
                cal(c);
            op_stack.pop();
        }
        if (symbol_stack.size() != 1)
            throw Expection("Location::regex", "invalid expression\n");

        // dbg
        // for (int i = 0;i < res.size();i++) {
        //     if (!is_symbol(res[i])) {
        //         switch (res[i])
        //         {
        //         case op::And:
        //             res[i] = '&';
        //             break;
        //         case op::closure:
        //             res[i] = '*';
        //             break;
        //         case op::Left:
        //             res[i] = '(';
        //             break;
        //         case op::Or:
        //             res[i] = '|';
        //             break;
        //         case op::Right:
        //             res[i] = ')';
        //             break;
        //         default:
        //             break;
        //         }
        //     }
        // }
        // dbg
        std::string res = symbol_stack.top();
        // for (int i = 0;i < res.size();i++)
        //{
        //     if (is_symbol(res[i])) std::cout << res[i] ;
        //     else std::cout << "?";
        // }
        // if (res.size() == 0)
        //     res+=symbol_stack.top();
        return symbol_stack.top();
    }
}
