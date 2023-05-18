#include"NFA.h"
#include<set>
#include<queue>
#include<iostream>
namespace regular_expression
{
	NFA::NFA()
	{
		head = tail = nullptr;
	}
	NFA::NFA(const char &c)
	{
		head = new node();
		tail = new node();
		head->link(tail, c);
	}
	NFA::NFA(node* _head, node* _tail)
		:head(_head), tail(_tail)
	{};

	node& NFA::get_head()
	{
		return *head;
	}
	node& NFA::get_tail()
	{
		return *tail;
	}

	std::string NFA::deep_to_string()
	{
		std::cout << "-------------------------------------\n";
		//std::string res;
		std::set<node*>vis;
		std::queue<node*>q;
		q.push(head);
		vis.insert(head);
		while (!q.empty())
		{
			auto cur = q.front();
			q.pop();
			//if (vis.count(cur)) continue;
			//else vis.insert(cur);
			std::cout << "node_number: " << cur <<" is_acceptable?"<<(cur->is_acceptable()?"true":"false") << "\n";
			std::cout << "linked edges: \n";
			for (auto it : cur->next_list)
			{
				if (vis.count(it.first) == 0)
				{
					q.push(it.first);
					vis.insert(it.first);
				}
				std::cout << "<Next_node: " << it.first << ", edge_value: ";
				if (it.second == Epsilon) std::cout << "epsilon";
				else std::cout << it.second;
				std::cout << ">\n";
			}
			std::cout << "\n";
		}
		std::cout << "-------------------------------------";
		return "";
	}
}