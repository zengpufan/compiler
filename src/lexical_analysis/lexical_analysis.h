/*
 * @Author: zpf 1600493127@qq.com
 * @Date: 2023-04-14 22:53:47
 * @LastEditors: zpf 1600493127@qq.com
 * @LastEditTime: 2023-05-08 12:29:08
 * @FilePath: \grammar-analysis\src\lexical_analysis\lexical_analysis.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
#include "regular_expression_engine.h"
#include "token_types.h"
#include <list>
#include <fstream>
#include <iostream>
#include "token.h"
namespace compiler
{
	typedef regular_expression::regular_expression_engine regex;

	class lexical_analysis
	{

	private:
		std::list<std::pair<regex, int>> regexes;
		// regular_expression::regular_expression_engine ;
	public:
		lexical_analysis(token_types *);
		int cur_state();
		bool look_forward(const char &c);
		bool push_back_char(const char &c);
		bool clear();
		std::list<token> Run(const std::list<std::string> &);
	};
}