#pragma once
#include <list>
namespace regular_expression
{

	class node
	{
	protected:
		bool acceptable;

	public:
		std::list<std::pair<node *, char>> next_list;
		node(bool _acceptable = false) : acceptable(_acceptable){};
		void set_acceptable(bool _ac)
		{
			acceptable = _ac;
		}
		inline bool is_acceptable() { return acceptable; };
		bool link(node *dis, char c)
		{
			next_list.push_back(std::make_pair(dis, c));
			return true;
		}
	};
}