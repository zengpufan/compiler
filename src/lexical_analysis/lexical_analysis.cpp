/*
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-04-16 23:58:03
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-08 23:14:53
 * @FilePath: \grammar-analysis\src\lexical_analysis\lexical_analysis.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "regular_expression_engine.h"
#include "lexical_analysis.h"
#include <list>
#include <fstream>
#include <iostream>
#include "expection.h"
#include "token.h"
#include "ErrorMessage.h"
namespace compiler
{
	typedef regular_expression::regular_expression_engine regex;

	lexical_analysis::lexical_analysis(token_types *_token_types)
	{
		try
		{
			for (auto &it : *_token_types)
			{
				regexes.push_back({it.get_regex(), it.get_id()});
			}
		}
		catch (Expection &e)
		{
			e.print();
			throw Expection("Location::lexical_analysis", "lexical_analysis failed, errors in regex");
		}
	}
	int lexical_analysis::cur_state()
	{
		for (auto &it : regexes)
		{
			if (it.first.is_matched())
			{
				return it.second;
			}
		}
		return -1;
	}
	bool lexical_analysis::look_forward(const char &c)
	{
		for (auto &it : regexes)
		{
			if (it.first.look_forward(c))
				return true;
		}
		return false;
	}
	bool lexical_analysis::push_back_char(const char &c)
	{
		for (auto &it : regexes)
		{
			it.first.push_back_char(c);
		}
		return true;
	}
	bool lexical_analysis::clear()
	{
		for (auto &it : regexes)
		{
			it.first.clear_string();
		}
		return true;
	}
	std::list<token> lexical_analysis::Run(const std::list<std::string> &ls)
	{
		std::list<token> result;
		int cnt_line = 0;
		for (auto &s : ls)
		{
			cnt_line++;
			int pre = 0;
			for (int i = 0; i < s.size(); i++)
			{
				this->push_back_char(s[i]);
				if (i + 1 != s.size() && this->look_forward(s[i + 1]))
					continue;
				int res = this->cur_state();
				if (res == -1)
					continue;
				else
				{
					result.push_back(token(res, s.substr(pre, i - pre + 1), location(cnt_line, 0)));
					// result.push_back(std::make_pair(symbol_list::get_symbol_by_id(res), s.substr(pre, i - pre + 1)));
					// std::cout << "token: <" << symbol_list::get_name_by_id(res) << "," << s.substr(pre, i - pre + 1) << "> \n";
					pre = i + 1;
					this->clear();
				}
			}
			if (pre != s.size())
			{
				ErrorMessage::addErrorMessage("lexical_analysis: invalid mark",location(cnt_line,0));
			}
			this->clear();
		}
		return result;
	}
}