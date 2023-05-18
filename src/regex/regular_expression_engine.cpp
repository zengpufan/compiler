#include "regular_expression_engine.h"
#include "regular_expression_preprocessor.h"
#include "algrithm.h"
#include <string>
namespace regular_expression
{
	regular_expression_engine::regular_expression_engine(const std::string &s)
		: cur_state(nullptr), cur_acceptable(false)
	{
		set_regex(s);
	}
	regular_expression_engine::regular_expression_engine()
		: cur_state(nullptr), cur_acceptable(false)
	{
	}
	bool regular_expression_engine::set_regex(const std::string &s)
	{
		typedef regular_expression_preprocessor preprocessor;
		std::string expression = preprocessor::infix2postfix(preprocessor::prework(s));
		dfa = Thompson(expression);
		// dfa.deep_to_string();
		dfa = subset_construction(dfa);

		dfa = minimum_DFA(dfa);
		cur_state = dfa.head;
		return true;
	}

	bool regular_expression_engine::set_string(const std::string &s)
	{
		if (dfa.head == nullptr)
			return false;
		cur_state = dfa.head;
		for (int i = 0; i < s.size(); i++)
		{
			char c = s[i];
			bool flag = false;
			for (auto &it : cur_state->next_list)
			{
				if (it.second == c)
				{
					cur_state = it.first;
					flag = true;
					break;
				}
			}
			if (!flag)
			{
				cur_state = nullptr;
				break;
			}
		}
		// if (cur_state == nullptr || cur_state->is_acceptable() == false)
		//	this->cur_acceptable = false;
		// else
		//	this->cur_acceptable = true;
		return true;
	}
	bool regular_expression_engine::clear_string()
	{
		cur_state = dfa.head;
		// this->cur_acceptable = cur_state->is_acceptable();
		return true;
	}
	bool regular_expression_engine::push_back_char(const char &c)
	{
		if (cur_state == nullptr)
			return true;
		bool flag = false;
		for (auto &it : cur_state->next_list)
		{
			if (it.second == c)
			{
				cur_state = it.first;
				flag = true;
				break;
			}
		}
		if (!flag)
		{
			cur_state = nullptr;
		}
		return true;
	}
	bool regular_expression_engine::is_matched()
	{
		if (cur_state == nullptr)
			return false;
		return cur_state->is_acceptable();
	}
	bool regular_expression_engine::look_forward(const char &c)
	{
		if (cur_state == nullptr)
			return false;
		for (auto &it : cur_state->next_list)
		{
			if (it.second == c)
			{
				return it.first->is_acceptable();
			}
		}
		return false;
	}
}