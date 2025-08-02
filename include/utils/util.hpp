#pragma once

#include "../list/list.hpp"

// #define extends public
// #define implements public

// template <typename T>
// void swap(T& a, T& b) {
// 	T aux = a;
// 	a = b;
// 	b = aux;
// }

/**
 * @brief Verifica se 'list' está ordenado em O(n)
 *
 * @param list lista a verificar.
 * @return bool Resultado da verificação, ordenado / não ordenado.
 */
template <typename T>
bool isSorted(List<T>& list) {

	if (list.size() == 0) return true;

	T previous = list[0];

	for (size_t i = 1; i < list.size(); i++) {

		T current = list[i];
		if (current < previous) return false;
		previous = current;
	}

	return true;
}
