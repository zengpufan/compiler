#pragma once
#include "NFA.h"
#include "DFA.h"
#include <string>
namespace regular_expression
{
	class regular_expression_engine
	{
	private:
		node *cur_state;
		bool cur_acceptable;
		DFA dfa;

	public:
		regular_expression_engine(const std::string &);
		regular_expression_engine();
		bool set_regex(const std::string &);
		bool clear_string();
		bool set_string(const std::string &);
		bool push_back_char(const char &);
		bool is_matched();
		bool look_forward(const char &);
	};
}