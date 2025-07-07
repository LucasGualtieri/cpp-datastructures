#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/tree/AVL.hpp"
#include "../include/tree/node.hpp"

using namespace std;

// clear && g++ test_AVL.cpp -std=c++23 && ./a.out

#define RANDOM(minRange, maxRange) (rand() % ((maxRange + 1) - minRange)) + minRange

int main() {

	const bool DEBUGGING = false;

	AVL<int> tree(DEBUGGING);

	srand(time(NULL));

	for (int i = 0; i < 1'000'000; i++) {
		int numero = rand() % 2000000;
		tree.insert(numero);
	}

	cout << format("Tree Height: {}", tree.height()) << endl;

	Node<int>* search;

	if ((search = tree.search(rand() % 2000000)) != nullptr) {
		printf("Valor %d encontrado.\n", search->value);
	}

	else cout << "Valor não encontrado." << endl;

	// tree.str();

	return 0;
}
