/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 18:37:41 by dperez-p          #+#    #+#             */
/*   Updated: 2026/07/30 15:02:33 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

// Load the reference database as soon as the object is created
BitcoinExchange::BitcoinExchange()
{
	loadDataBase("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	*this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		_rates = other._rates;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{}

// Remove leading, spaces and tabs etc usually called trim
std::string BitcoinExchange::skipSpaces(const std::string& str) const
{
	size_t	start = str.find_first_not_of(" \t");
	size_t	end = str.find_last_not_of(" \t");
	if (start == std::string::npos)
		return "";
	return	str.substr(start, end - start + 1);
}

void	BitcoinExchange::loadDataBase(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		std::exit(1);
	}

	std::string line;
	std::getline(file, line); // skip header

	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		size_t	commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue ;
		std::string date = line.substr(0, commaPos);
		std::string rateStr = line.substr(commaPos + 1);
		double	rate = std::atof(rateStr.c_str()); // c_str transfor the string on array of chars
		_rates[date] = rate; // create or check if exist with the keyworld and add the value
	}
	file.close();
}

//Check conditions needed for the date
bool	BitcoinExchange::isValidDate(const std::string& date) const
{
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;
	for (size_t i = 0; i < date.size(); i++)
	{
		if (i == 4 || i == 7)
			continue ;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}
	int	year = std::atoi(date.substr(0, 4).c_str());
	int	month = std::atoi(date.substr(5, 2).c_str());
	int	day	= std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > 31)
		return false;

	int		daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	bool	isLeap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)); // secular year one wich close the century
	if (isLeap)
		daysInMonth[1] = 29;
	if (day > daysInMonth[month - 1])
		return false;
	return true;
}

// Parses the value and checks it's a well-formed number
bool BitcoinExchange::isValidValue(const std::string& valueStr, double& value) const
{
	if (valueStr.empty())
	{
		return false;
	}
	char* end;
	value = std::strtod(valueStr.c_str(), &end);
	//if empty or leftover characters
	if (end == valueStr.c_str() || *end != '\0')
	{
		return false;
	}
	if (std::isnan(value))
		return false;
	return true;
}

void	BitcoinExchange::processInputFile(const std::string& filename) const
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return ;
	}
	std::string line;
	std::getline(file, line);
	if (line != "date | value")
	{
		std::cout << "Error: Missing header." << std::endl;
		return ;
	}
	//bool	datafound = false;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			std::cout << "Error: empty line." << std::endl;
			continue;
		}
		size_t sepPos = line.find('|');
		if (sepPos == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		std::string	date = skipSpaces(line.substr(0, sepPos));
		std::string valueStr = skipSpaces(line.substr(sepPos + 1));
		//datafound = true;
		if (!isValidDate(date))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		double	value;
		if (!isValidValue(valueStr, value))
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (value < 0)
		{
			std::cout << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cout << "Error: too large a number." << std::endl;
			continue;
		}
		//find the exat date, or the closest one before
		std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);

		if (it == _rates.end() || it->first != date)
		{
			if (it == _rates.begin())
			{
				std::cout << "Error: no data available for date => " << date << std::endl;
				continue;
			}
			--it;
		}
		double	result = value * it->second;
		std::cout << date << " => " << valueStr << " = " << result << std::endl;
	}
	//if (!datafound)
	//{
	//	std::cout << "Error: file is empty or has no data." << std::endl;
	//	return ;
	//}
	
	file.close();
}
