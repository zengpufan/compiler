#pragma once
#include<string>
#include"NFA.h"
#include"DFA.h"
#include<set>
namespace regular_expression
{
	NFA Thompson(const std::string&);
	DFA subset_construction(const NFA&);
	DFA minimum_DFA(const DFA&);
	//std::set<node*> e_closure(node*);

	std::set<node*>& e_closure(std::set<node*>&);
	std::set<node*> Move(const std::set<node*>&,const char&);
	int hash_algrithm(const std::set<node*>&);
	int hash_algrithm(const int&,const int&);
	void _show(const std::string&);
}