/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 08:55:39 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/02 12:19:50 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN()
{
}

RPN::~RPN()
{
}

// function to calculate operations
int	RPN::calculate(const std::string& expression)
{
	std::istringstream	iss(expression);
	std::string			token;

	while (iss >> token)
	{
		if (isNumber(token))
		{
			_stack.push(std::atoi(token.c_str()));
		}
		else if (isOperator(token))
		{
			if (_stack.size() < 2)
			{
				throw std::runtime_error("not enought operands, malformed expression.");
			}
			int	b = _stack.top(); //extract
			_stack.pop(); //delete actual
			int	a = _stack.top();
			_stack.pop();
			int	result = applyOperator(token, a, b);
			_stack.push(result);
		}
		else
		{
			throw std::runtime_error("Invalid token.");
		}
	}
	//stack must to ahve only one element
	if (_stack.size() != 1)
	{
		throw std::runtime_error("malformed expression.");
	}
	return _stack.top();
}

// bool for the operator
bool RPN::isOperator(const std::string& token) const
{
	if (token == "+" || token == "-" || token == "/" || token == "*")
	{
		return true;
	}
	return false;
}

// bool for valid numbers
bool RPN::isNumber(const std::string& token) const
{
	if (token.empty())
		return false;
	for (size_t i = 0; i < token.length(); i++)
	{
		if (token.length() > 1 && i == 0)
		{
			if (token[i] == '-' && isdigit(token[i + 1]))
			{
				continue ;
			}
		}
		if (!isdigit(token[i]))
		{
			return false;
		}
	}
	return true;
}

//aply the operator, we need to be careful with the order, a is the older
int	RPN::applyOperator(const std::string& op, int a, int b) const
{
	switch (op[0])
	{
		case '+':
			return (a + b);
		case '-':
			return (a - b);
		case '*':
			return (a * b);
		case '/':
			if (b == 0)
			{
				throw std::runtime_error("division by zero.");
			}
			return (a / b);
		default:
			return 0;
	}
}
