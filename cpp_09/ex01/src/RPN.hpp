/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 08:55:39 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/02 12:29:25 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <stack>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstdlib>

class RPN
{
private:
	std::stack<int>	_stack; //container

	bool	isOperator(const std::string& token) const; //chekcs if a token is = - / *

	bool	isNumber(const std::string& token) const; //checks if is a valid number

	int		applyOperator(const std::string& op, int a, int b) const;


public:
	RPN();
	~RPN();

	int	calculate(const std::string& expression);
};

