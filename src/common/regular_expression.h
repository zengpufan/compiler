#pragma once
namespace regular_expression
{

	enum op
	{
		Right = 1,
		Or = 2,
		And = 3,
		closure = 4,
		Left = 5,
		Lsquare = 6,
		Rsquare = 7
	};
	enum
	{
		op_count = 7,
		wild = 6,
		Epsilon = 7,
		symbol_set_size = 85
	};
	const char symbol_set[] =
		{
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'+', '-', '*', '/', '!', '%', '^', '&', '|',
			'>', '<', '=', '.',
			'(', ')', '[', ']', '{', '}',
			',', ';', '\"','$'
			// 26+26+10+9+4+6+3=85;
	};
	inline static bool is_op(const char &c) { return (c >= 1 && c <= 7) ? true : false; }

}