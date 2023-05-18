#pragma once
#include"node.h"
#include"regular_expression.h"
#include<string>
namespace regular_expression
{

	class NFA
	{
	public:
		node* head=nullptr;
		node* tail=nullptr;
		
	public:
		NFA();
		NFA(const char&c);
		NFA(node*_head, node*_tail);
		node& get_head();
		node& get_tail();


		std::string deep_to_string();
	};
}