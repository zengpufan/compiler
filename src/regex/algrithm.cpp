#include"NFA.h"
#include"DFA.h"
#include<string>
#include<stack>
#include<set>
#include<stdlib.h>
#include<map>
#include<iostream>
#include<queue>
#include<algorithm>
#include"regular_expression_preprocessor.h"
#include"regular_expression.h"
namespace regular_expression
{
	int hash_algrithm(const std::set<node*>& se)
	{
		const int hash_code = 1e9 + 7;
		int res = 0;
		for (auto it : se)
		{
			res = res * hash_code + (long long)it;
		}
		return res;
	}
	int hash_algrithm(const int& x, const int& y)
	{
		return x * 998244353+y;
	}
	NFA Thompson(const std::string&s)
	{
		std::stack<NFA>NFA_stack;
		std::stack<char>op_stack;
		auto construct_and = [&]()
		{
			NFA y = NFA_stack.top();
			NFA_stack.pop();
			NFA x = NFA_stack.top();
			NFA_stack.pop();
			x.get_tail().link(&y.get_head(), Epsilon);
			x.tail = y.tail;
			NFA_stack.push(x);
		};
		auto construct_or = [&]()
		{
			NFA x = NFA_stack.top();
			NFA_stack.pop();
			NFA y = NFA_stack.top();
			NFA_stack.pop();
			node* start = new node;
			node* end = new node;
			start->link(x.head, Epsilon);
			start->link(y.head, Epsilon);
			x.tail->link(end, Epsilon);
			y.tail->link(end, Epsilon);
			NFA_stack.push(NFA(start, end));
		};
		auto construct_closure = [&]()
		{
			NFA x = NFA_stack.top();
			NFA_stack.pop();
			x.tail->link(x.head, Epsilon);
			node* start = new node;
			node* end = new node;
			start->link(x.head, Epsilon);
			start->link(end, Epsilon);
			x.tail->link(end, Epsilon);
			NFA_stack.push(NFA(start, end));
		};
		for (int i = 0;i < s.size();i++)
		{
			if (is_op(s[i]))
			{
				switch (s[i])
				{
				case op::And:
					construct_and();
					break;
				case op::closure:
					construct_closure();
					break;
				case op::Or:
					construct_or();
					break;
				default:
					break;
				}
			}
			else
			{
				NFA_stack.push(NFA(s[i]));
			}
		}
		NFA res = NFA_stack.top();
		res.get_tail().set_acceptable(true);
		return res;
	}

	std::set<node*>& e_closure(std::set<node*>& se)
	{
		while (true)
		{
			std::stack<node*>new_state;
			for (auto node : se)
			{
				for (auto edge : (*node).next_list)
				{
					if (edge.second != Epsilon) continue;
				
					if (se.count(edge.first) == 0)
						new_state.push(edge.first);
				}
			}

			if (new_state.empty())
				break;

			while (!new_state.empty())
			{
				se.insert(new_state.top());
				new_state.pop();
			}
		}

		return se;
	}

	//Do not modify se,but don't return referrence
	std::set<node*> Move(const std::set<node*>&se, const char& c)
	{
		//std::stack<node*>new_state;
		std::set<node*>res;
		for (auto node : se)
		{
			for (auto edge : (*node).next_list)
			{
				if (edge.second != c)
					continue;
				res.insert(edge.first);
			}
		}
		//res = se;
		//while (!new_state.empty())
		//{
		//	res.insert(new_state.top());
		//	new_state.pop();
		//}
		return res;
	}

	DFA subset_construction(const NFA& nfa)
	{
		typedef int hash_state;

		//std::set<hash_state>visited_state_set;
		std::set<std::set<node*>>unvisited_state_set;
		std::set<node*>s0;
		//map the hash_state with its address.
		//the state in the mp is the same as the state in the Dstate
		std::map<hash_state, node*>mp;
		DFA res;

		res.head = new node();

		s0.insert(nfa.head);
		e_closure(s0);
		mp[hash_algrithm(s0)] = res.head;
		//tansform s0 to rvalue,to call move constructor
		unvisited_state_set.insert(std::move(s0));
		
		while (!unvisited_state_set.empty())
		{
			
			std::set<node*>T = *unvisited_state_set.begin();
			int hash_T = hash_algrithm(T);
			unvisited_state_set.erase(unvisited_state_set.begin());
			//if there exist acceptale state,mark T acceptable
			for (auto it : T)
			{
				if (it->is_acceptable())
				{
					mp[hash_T]->set_acceptable(true);
					break;
				}
			}


			for (int i = 0;i < symbol_set_size;i++)
			{
				char c = symbol_set[i];
				std::set<node*>U = Move(T, c);
				if (U.empty()) continue;
				e_closure(U);
				
				int hash_U = hash_algrithm(U);
				//if (hash_U == hash_T) continue;

				if (mp.count(hash_U) == 0)
				{
					unvisited_state_set.insert(move(U));
					//U has not been created
					node* node_U = new node();
					mp[hash_T]->link(node_U, c);
					mp[hash_U] = node_U;
				}
				else
				{
					//U has been created
					mp[hash_T]->link(mp[hash_U], c);
				}
			}
		}
		return res;
	}
	DFA minimum_DFA(const DFA& dfa)
	{
		std::map<node*,int>cur_states;
		std::set<node*>vis;			
		int cnt_set = 0;
		auto dfs = [&](auto self, node* cur)->void
		{
			if (cur->is_acceptable()) 
				cur_states[cur]=1;
			else
			{
				cur_states[cur]=0;
				cnt_set = 1;
			}
				
			for (auto it = cur->next_list.begin();it != cur->next_list.end();it++)
			{
				if (vis.count(it->first) == 0)
				{
					vis.insert(it->first);
					self(self, it->first);
				}
			}
		};
		dfs(dfs, dfa.head);
		cnt_set++;
		//auto hash = [&](int x, int y) {return x * 998244353 + y;};
		while (true)
		{
			std::map<node*, int>next_states;
			std::set<int>set_counter;
			for (auto it = cur_states.begin();it != cur_states.end();it++)
			{
				int cur_hash_code = it->second;
				//next node's status, passway
				std::vector<std::pair<int, int>>v;
				for (const auto &Next : it->first->next_list)
				{
					v.push_back(std::make_pair(cur_states[Next.first], Next.second));
				}
				std::sort(v.begin(), v.end());
				for (const auto& Next : v)
				{
					cur_hash_code = hash_algrithm(cur_hash_code, Next.first);
					cur_hash_code = hash_algrithm(cur_hash_code, Next.second);
				}
				next_states[it->first] = cur_hash_code;
				set_counter.insert(cur_hash_code);
			}
			cur_states = std::move(next_states);
			if (cnt_set == (int)set_counter.size()) 
				break;
			else 
				cnt_set = (int)set_counter.size();
		}
		//construct new dfa
		node* head = nullptr;
		node* tail = nullptr;
		std::map<int, node*>hashCode_NewDFANode;
		for (const auto &it : cur_states)
		{
			if (hashCode_NewDFANode.count(it.second) == 0)
			{
				node* temp = new node;
				if (it.first->is_acceptable())
					temp->set_acceptable(true);
				hashCode_NewDFANode[it.second] = temp;
			}	
		}
		head = hashCode_NewDFANode[cur_states[dfa.head]];
		std::map<int, std::set<std::pair<char, int>>>hashCode_NewEdge;
		for (const auto& it : cur_states)
		{
			for (const auto& Next : it.first->next_list)
			{
				hashCode_NewEdge[it.second].insert(std::make_pair(Next.second,cur_states[ Next.first]));
			}
		}
		for (const auto& it : hashCode_NewEdge)
		{
			for (const auto& edge : it.second)
			{
				node* next_node = hashCode_NewDFANode[edge.second];
				char edge_char = edge.first;
				hashCode_NewDFANode[it.first]->link(next_node, edge_char);
			}
		}
		return DFA(head, tail);
	}
	void _show(const std::string& s)
	{
		using namespace regular_expression;
		std::string str=regular_expression_preprocessor::infix2postfix(regular_expression_preprocessor::prework(s));
		NFA nfa = Thompson(str);
		nfa.deep_to_string();
		DFA dfa = subset_construction(nfa);
		dfa.deep_to_string();
		dfa=minimum_DFA(dfa);
		dfa.deep_to_string();
	}
}