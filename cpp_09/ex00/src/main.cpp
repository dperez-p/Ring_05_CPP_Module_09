/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 18:37:29 by dperez-p          #+#    #+#             */
/*   Updated: 2026/07/19 18:50:15 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int	main(int ac, char** av)
{
	if (ac != 2)
	{
		std::cerr << "Error: Could not open file." << std::endl;
		return 1;
	}
	BitcoinExchange btc;
	btc.processInputFile(av[1]);

	return (0);
}
