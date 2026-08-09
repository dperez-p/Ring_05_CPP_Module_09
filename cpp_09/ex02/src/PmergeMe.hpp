/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 18:06:48 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/08 17:18:12 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La idea del algoritmo Frod-Johnson, en plan sencillo:

Agrupas los números de dos en dos. En cada pareja te quedas con cuál es el "grande" y cuál el "pequeño".
Ordenas recursivamente solo los "grandes" (esto es lo que lo hace mergesort-like).
Metes los "pequeños" en esa cadena ya ordenada, uno a uno, con búsqueda binaria — pero en un orden especial (números de Jacobsthal) que minimiza comparaciones.
El primer "pequeño" (b1) se coloca gratis al principio, sin comparar, porque ya sabemos que es el menor de todos. */
#pragma once

#include <vector>
#include <list>
#include <utility>
#include <cstddef>
#include <algorithm>
#include <limits>

class PmergeMe
{
private:
	std::vector<int> _values;

	//recursive sort
	std::vector<size_t> vectorRecSort(std::vector<size_t> index);

	//insert every small index from pairs
	void	vectorInsertPending(std::vector<size_t>& chain, std::vector<std::pair<size_t, size_t> >& pairs);
	//binary search
	size_t	vectorFindInsertPos(const	std::vector<size_t>& chain, size_t valueIdx, size_t searchLimit) const;
	//recursive sort on list
	std::list<size_t> listRecSort(std::list<size_t> chunk);
	// linear walk (bounded by the big partner's position)
	void	listInsertPending(std::list<size_t>&chain, std::vector<std::pair<size_t, size_t> >& pairs);

	// Returns the insertion order for b2, b3, ..., bn (b1 is handled
	// separately, always first). Based on the Jacobsthal sequence:
	static	std::vector<size_t> jacobsthalGaps(size_t n);
public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	void	sortWithVector(std::vector<int>& input);
	void	sortWithList(std::list<int>& input);
};
