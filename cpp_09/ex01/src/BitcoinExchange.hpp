/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 18:37:47 by dperez-p          #+#    #+#             */
/*   Updated: 2026/07/19 19:30:40 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* STD MAP guarda pares clave-valor ordenados automaticamente pro la clave*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


/*std::map<key to search, value of the key>	;*/
class BitcoinExchange
{
private:
	std::map<std::string, double>	_rates; //with map we can decide the key(string)

	void		loadDataBase(const std::string& filename);
	bool		isValidDate(const std::string& date) const;
	bool		isValidValue(const std::string& valueStr, double&	value) const;
	std::string	skipSpaces(const std::string& str) const;

public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& other);
	BitcoinExchange& operator=(const BitcoinExchange& other);
	~BitcoinExchange();

	void	processInputFile(const std::string& filename) const;
};
