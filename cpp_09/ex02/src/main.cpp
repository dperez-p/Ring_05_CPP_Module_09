/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 18:06:45 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/08 17:20:19 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <climits>
#include <iostream>
#include <sys/time.h>
#include <time.h>

static bool parsePositiveInt(const std::string& token, int& out)
{
	if (token.empty())
	{
		return false;
	}
	size_t	start = 0;
	if (token[start] == '+')
		start++;
	if (start == token.size())
		return false;
	while (start < token.size())
	{
		if (!isdigit(static_cast<unsigned char>(token[start]))) // evading negative value characters.
		{
			return false;
		}
		start++;
	}
	long value = std::strtol(token.c_str(), NULL, 10);
	if (value <= 0 || value > INT_MAX)
		return false;
	out = static_cast<int>(value);
	return true;
}

// Converts a struct timeval into a single "total microseconds" value,
// so two timestamps can just be subtracted.
static double toMicro(const struct timespec& ts)
{
    return static_cast<double>(ts.tv_sec) * 1000000.0 + static_cast<double>(ts.tv_nsec) / 1000.0;
}

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	std::vector<int> posParse;
	for (int i = 1; i < ac; i++)
	{
		int value = 0;
		if (!parsePositiveInt(av[i], value))
		{
			std::cerr << "Error" << std::endl;
			return 1;
		}
		posParse.push_back(value);
	}

	std::cout << "Before:";
	for (size_t i = 0; i < posParse.size(); i++)
		std::cout << " " << posParse[i];
	std::cout << std::endl;

	std::vector<int> vecCopy(posParse.begin(), posParse.end());
	std::list<int> listCopy(posParse.begin(), posParse.end());

	// ---- sort with std::vector ----
	PmergeMe sorterVec;
	struct timespec startVec, endVec;
	clock_gettime(CLOCK_MONOTONIC, &startVec);
	sorterVec.sortWithVector(vecCopy);
	clock_gettime(CLOCK_MONOTONIC, &endVec);
	double micro = toMicro(endVec) - toMicro(startVec);

	// ---- sort with std::list ----
	PmergeMe sorterList;
	struct timespec startList, endList;
	clock_gettime(CLOCK_MONOTONIC, &startList);
	sorterList.sortWithList(listCopy);
	clock_gettime(CLOCK_MONOTONIC, &endList);
	double microList = toMicro(endList) - toMicro(startList);

	std::cout << "After:";
	for (size_t i = 0; i < vecCopy.size(); i++)
		std::cout << " " << vecCopy[i];
	std::cout << std::endl;

	std::cout << "Time to process a range of " << posParse.size()
		<< " elements with std::vector : " << micro << " us" << std::endl;
	std::cout << "Time to process a range of " << posParse.size()
		<< " elements with std::list : " << microList << " us" << std::endl;
	return 0;
}

