/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 18:06:35 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/08 17:19:17 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <algorithm>

PmergeMe::PmergeMe()
{}

PmergeMe::PmergeMe(const PmergeMe& other)
{
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		_values = other._values;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{}

//Jacobsthal insertion.
std::vector<size_t> PmergeMe::jacobsthalGaps(size_t n)
{
	std::vector<size_t> order; // return value
	if (n < 2)
		return order;
	/* mark where one insertion group ends and the next begin */
	std::vector<size_t> t;
	t.push_back(1);
	t.push_back(3);
	while (t.back() < n)
	{
		t.push_back(t[t.size() - 1] + 2 * t[t.size() - 2]);  // example: t = {1, 3, 5} || t[t.size() - 1] = 5 || t[t.size() - 2] = 3 || 5 + 2 * 3 = 11 || t.push_back(11);
	}
	// lower = 2 because b1 is never handled here (it's inserted separately, for free, before this function is even called).
	size_t lower = 2;
	// k starts at 1 (not 0) because t[0]=1 was only the seed value
	// used to compute t[1]=3; the first real "checkpoint" we care
	// about is t[1].
	for (size_t k = 1; k < t.size() && lower <= n; k++)
	{
		size_t upper = t[k];
		if (upper > n)
		{
			upper = n; // don't go past the last real b-index
		}
		// walk from upper down to lower, pushing each index
		for (size_t idx = upper; idx >= lower; idx--)
		{
			order.push_back(idx);
			if (idx == lower) // dont let idx-- wrap around below 0
				break ;
		}
		lower = t[k] + 1; //next group;
	}
	return order;
}

//search vector possition to insert,
size_t PmergeMe::vectorFindInsertPos(const std::vector<size_t>& chain, size_t valueIdx, size_t searchLimit) const
{
	size_t lo = 0; //start range
	size_t hi = searchLimit; //limit of the range
	while (lo < hi)
	{
		size_t mid = lo + (hi - lo) / 2; //recalculating every loop
		if (_values[valueIdx] > _values[chain[mid]])
			lo = mid + 1;
		else
			hi = mid;
	}
	return lo;
}

void PmergeMe::vectorInsertPending(std::vector<size_t>& chain, std::vector<std::pair<size_t, size_t> >& pairs)
{
	std::vector<size_t> bigOrder = chain; // freeze the current order of big.
	std::vector<size_t> smallOf(_values.size(), std::numeric_limits<size_t>::max()); //vector small of original index
	for (size_t i = 0; i < pairs.size(); i++)
	{
		smallOf[pairs[i].first] = pairs[i].second;// second one is the slammest, saving the small to position 1 of the pair
	}
	chain.insert(chain.begin(), smallOf[bigOrder[0]]); // this number is allways smaller than the small bignumber.
	std::vector<size_t> order = jacobsthalGaps(pairs.size());
	for (size_t i = 0; i < order.size(); i++)
	{
		size_t k = order[i];
		size_t bixIdx = bigOrder[k - 1]; // first of each pair is the bigger in bigOrder
		size_t smallIdx = smallOf[bixIdx]; // its corresponding little one
		std::vector<size_t>::iterator bigPos = std::find(chain.begin(), chain.end(), bixIdx);
		size_t	limit = bigPos - chain.begin(); // limit of movements from begin to the bixpos index.
		size_t	pos = vectorFindInsertPos(chain, smallIdx, limit); // search poss to insert
		chain.insert(chain.begin() + pos, smallIdx); // insert from the begin + pos searched, the smallIdx
	}
}


void	PmergeMe::sortWithVector(std::vector<int>& input)
{
	_values = input;
	std::vector<size_t> index;
	for (size_t i = 0; i < input.size(); i++)
	{
		index.push_back(i);
	}
	std::vector<size_t> result = vectorRecSort(index);
	for (size_t i = 0; i < result.size(); i++)
	{
		input[i] = _values[result[i]];
	}
}


// Main Vect sort function, first the bigger and then the rest, putting the last one if no pair.
std::vector<size_t> PmergeMe::vectorRecSort(std::vector<size_t> index)
{
	if (index.size() <= 1) //nothing to sort return index
	{
		return index;
	}
	size_t	straggler = 0;
	bool	straggle = false;
	if (index.size() % 2 != 0)
	{
		straggle = true;
		straggler = index.back();
		index.pop_back();
	}
	std::vector<std::pair<size_t, size_t> > pairs;
	std::vector<size_t> bigIndex;
	for (size_t i = 0; i < index.size(); i += 2)
	{
		size_t left = index[i];
		size_t right = index[i + 1];
		if (_values[left] > _values[right])
		{
			pairs.push_back(std::make_pair(left, right));
		}
		else
			pairs.push_back(std::make_pair(right, left));
		bigIndex.push_back(pairs.back().first);
	}
	std::vector<size_t> chain = vectorRecSort(bigIndex);
	vectorInsertPending(chain, pairs);
	if (straggle)
	{
		size_t pos = vectorFindInsertPos(chain, straggler, chain.size());
		chain.insert(chain.begin() + pos, straggler);
	}
	return chain;
}

/********************************************LIST**********************************************************************************/

void PmergeMe::listInsertPending(std::list<size_t>& chain, std::vector<std::pair<size_t, size_t> >& pairs)
{
	// start inserting things into chain (same idea as vector version).
	std::vector<size_t> bigOrder(chain.begin(), chain.end()); // freeze the current order of big.
	std::vector<size_t> smallOf(_values.size(), std::numeric_limits<size_t>::max()); //vector small of original index
	for (size_t i = 0; i < pairs.size(); i++)
	{
		smallOf[pairs[i].first] = pairs[i].second;// second one is the slammest, saving the small to position 1 of the pair
	}
	// reasoning as the vector version. push_front is list's O(1)
	chain.push_front(smallOf[bigOrder[0]]); // this number is allways smaller than the small bignumber.
	std::vector<size_t> order = jacobsthalGaps(pairs.size());
	for (size_t i = 0; i < order.size(); i++)
	{
		size_t k = order[i];
		size_t bixIdx = bigOrder[k - 1]; // first of each pair is the bigger in bigOrder
		size_t smallIdx = smallOf[bixIdx]; // its corresponding little one
		std::list<size_t>::iterator pos = chain.begin();

		// Walk from the front while the current value is still
		while (pos != chain.end() && _values[*pos] < _values[smallIdx])
				++pos;
		chain.insert(pos, smallIdx);
	}
}


void	PmergeMe::sortWithList(std::list<int>& input)
{
	_values = std::vector<int>(input.begin(), input.end()); // ramge constructor to vector from list.
	std::list<size_t> index;
	for (size_t i = 0; i < input.size(); i++)
	{
		index.push_back(i);
	}
	std::list<size_t> result = listRecSort(index);

	std::list<int>::iterator it = input.begin();
	std::list<size_t>::iterator resultIt = result.begin();
	for (; resultIt != result.end(); resultIt++, it++)
	{
		*it = _values[*resultIt];
	}
}

/* Main list sort function: same Ford-Johnson logic as vectorRecSort,
but built with std::list, so pairing and the straggler search use
iterators instead of index-based [] access. */
std::list<size_t> PmergeMe::listRecSort(std::list<size_t> index)
{
	if (index.size() <= 1) //nothing to sort return index
	{
		return index;
	}
	size_t	straggler = 0;
	bool	straggle = false;
	if (index.size() % 2 != 0)
	{
		straggle = true;
		straggler = index.back();
		index.pop_back();
	}
	// pairs stays a vector on purpose: it's just bookkeeping, always
	// walked front-to-back by numeric index, never needs list-style
	// insertion in the middle.
	std::vector<std::pair<size_t, size_t> > pairs;
	std::list<size_t> bigIndex;

	// Walk two elements at a time with a manual iterator (list has
	// no operator[], so no index-based access is possible here).
	for (std::list<size_t>::iterator it = index.begin(); it != index.end(); )
{
		size_t left = *it;
		++it;
		size_t right = *it;
		++it;
		if (_values[left] > _values[right])
		{
			pairs.push_back(std::make_pair(left, right));
		}
		else
			pairs.push_back(std::make_pair(right, left));
		bigIndex.push_back(pairs.back().first);
	}
	std::list<size_t> chain = listRecSort(bigIndex);
	listInsertPending(chain, pairs);
	// Straggler goes in last: linear walk instead of binary search,
	// since list can't jump to an arbitrary position.
	if (straggle)
	{
		std::list<size_t>::iterator pos = chain.begin();
		while (pos != chain.end() && _values[*pos] < _values[straggler])
			++pos;
		chain.insert(pos, straggler);
	}
	return chain;
}
