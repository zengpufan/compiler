/*
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-05-03 10:04:10
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-08 12:35:50
 * @FilePath: \grammar-analysis\src\LL1\sytax_tree.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "sytax_tree.h"
#include <stack>
#include <algorithm>
#include "grammar_list.h"
namespace compiler
{
    sytax_node::sytax_node(int id, std::string value = "")
        : id(id), value(value) {}
    sytax_tree::sytax_tree(analyse_table &tb, std::list<token> &token_list)
    {
        // push $ into stack
        token_list.push_back(token(grammar_list::get_id_by_name("File_end"), "", location(0, 0)));

        std::stack<sytax_node *> analyse_stack;
        this->head = new sytax_node(grammar_list::start_id);
        analyse_stack.push(this->head);
        auto cur_token = token_list.begin();
        while (!analyse_stack.empty())
        {
            sytax_node *cur = analyse_stack.top();
            analyse_stack.pop();
            if (grammar_list::is_terminal(cur->id))
            {
                if (cur_token == token_list.end())
                {
                    std::cout << "invalid expression" << std::endl;
                    break;
                }
                if (cur->id == cur_token->get_id())
                {
                    cur_token = std::next(cur_token);
                }
            }
            else
            {
                int next_exp = tb.Move(cur->id, cur_token->get_id());
                if (next_exp == -1)
                {
                    std::cout << "invalid expression" << std::endl;
                    break;
                }
                auto exp = tb.get_exp_by_id(next_exp);

                std::reverse(exp.begin(), exp.end());
                for (auto it : exp)
                {
                    sytax_node *node = new sytax_node(it);
                    cur->ne.push_back(node);
                    analyse_stack.push(node);
                }
                std::reverse(cur->ne.begin(), cur->ne.end());
            }
        }
    }
    void sytax_tree::print()
    {
        using namespace std;
        cout << "< name , dest >" << endl;
        auto dfs = [&](auto self, sytax_node *p) -> void
        {
            cout << endl;
            cout << "node : "
                 << "< " << grammar_list::get_name_by_id(p->id) << " , " << p << " >" << endl;
            cout << "children : ";
            for (auto it : p->ne)
            {
                cout << " < " << grammar_list::get_name_by_id(it->id) << " , " << it << " > ";
            }
            cout << endl
                 << endl;
            for (auto it : p->ne)
            {
                self(self, it);
            }
        };
        dfs(dfs, this->head);
    }
    sytax_tree::~sytax_tree()
    {
        auto del = [&](auto self, sytax_node *p) -> void
        {
            if (p == nullptr)
                return;
            else
            {
                for (auto &it : p->ne)
                {
                    self(self, it);
                }
            }
            return;
        };
        del(del, this->head);
    }
}