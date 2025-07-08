#pragma once

#include <algorithm>
#include <cstddef>

template<typename T>
struct Node {

	T value;
	size_t level;
	Node<T> *left, *right;

	Node(int value) {
		this->value = value;
		this->level = 1;
		left = right = nullptr;
	}

	void updateLevel() {
		level = std::max(
			right ? right->level : 0,
			left ? left->level : 0
		) + 1;
	}

	int getFactor() {
		return (right ? right->level : 0) - (left ? left->level : 0);
	}
};
