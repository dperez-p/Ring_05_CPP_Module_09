/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dperez-p <dperez-p@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 18:06:48 by dperez-p          #+#    #+#             */
/*   Updated: 2026/08/02 19:09:50 by dperez-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* La idea del algoritmo, en plan sencillo:

Agrupas los números de dos en dos. En cada pareja te quedas con cuál es el "grande" y cuál el "pequeño".
Ordenas recursivamente solo los "grandes" (esto es lo que lo hace mergesort-like).
Metes los "pequeños" en esa cadena ya ordenada, uno a uno, con búsqueda binaria — pero en un orden especial (números de Jacobsthal) que minimiza comparaciones.
El primer "pequeño" (b1) se coloca gratis al principio, sin comparar, porque ya sabemos que es el menor de todos. */


#include <vector>
#include <list>
#include <utility>
#include <cstddef>

class PmergeMe
{
private:
	std::vector<int> _values;
	std::vector<size_t> vectorRecSort(std::vector<size_t> index);

	void	vectorInsertPending(std::vector<size_t>& chain, std::vector<std::pair<size_t, size_t> >& pairs);
public:
	PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);
	~PmergeMe();

	void	sortWithVector(std::vector<int>& input);
	void	sortWithList(std::list<int>& input);
};
