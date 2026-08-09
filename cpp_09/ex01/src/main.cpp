/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 08:55:29 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/08 17:47:33 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int	main(int ac, char** av)
{
	if (ac != 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	RPN school;

	try
	{
		double result = school.calculate(av[1]);
		std::cout << result << std::endl;
	}
	catch(const std::exception& e)
	{
		(void)e;
		std::cerr << "Error" << std::endl;
		return 1;
	}
	return 0;
}
