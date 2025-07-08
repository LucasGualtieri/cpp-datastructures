#pragma once

#include <cstdlib>
#include <format>
#include <iostream>
#include <stdexcept>

#include "node.hpp"

#define TESTE 1

template<typename T>
class AVL {

	Node<T>* root;
	const bool DEBBUGING;

	public:
	AVL(const bool& debbuging = false) : DEBBUGING(debbuging) {
		root = nullptr;
	}

	~AVL() {
		// freeNodes(root);
		// root = nullptr;
	}
	private:

	void freeNodes(Node<T>* node) {
		if (node != nullptr) {
			freeNodes();
			freeNodes(node->right);
			delete node;
		}
	}

	public:
	int height() {
		return height(root);
	}

	private:
	int height(Node<T>* node) {

		if (DEBBUGING && node == nullptr) {
			throw std::runtime_error("Erro ao calcular altura da árvore: Árvore vazia.");
		}

		return std::max(
			node->left ? node->left->level : 0,
			node->right ? node->right->level : 0
		) + 1;
	}

	Node<T>* Balance(Node<T>* node) {

		node->updateLevel();

		if (DEBBUGING) {
			std::cout << std::format("Node: {}, level: {},  factor: {}", node->value, node->level, node->getFactor()) << std::endl;
		}

		if (node->getFactor() == 2) {

			if (node->right->getFactor() == 1) {
				node = SimpleRotationLeft(node);
			}

			else node = DoubleRotationRightLeft(node);
		}

		else if (node->getFactor() == -2) {

			if (node->left->getFactor() == -1) {
				node = SimpleRotationRight(node);
			}

			else node = DoubleRotationLeftRight(node);
		}

		return node;
	}

	Node<T>* SimpleRotationLeft(Node<T>* node) {

		if (DEBBUGING) {
			std::cout << "SimpleRotationLeft" << std::endl;
		}

		Node<T>* aux = node->right;
		node->right = aux->left;
		aux->left = node;

		node->updateLevel();
		aux->updateLevel();

		return aux;
	}

	Node<T>* SimpleRotationRight(Node<T>* node) {

		if (DEBBUGING) {
			std::cout << "SimpleRotationRight" << std::endl;
		}

		Node<T>* aux = node->left;
		node->left = aux->right;
		aux->right = node;

		node->updateLevel();
		aux->updateLevel();

		return aux;
	}

	Node<T>* DoubleRotationRightLeft(Node<T>* node) {

		if (DEBBUGING) {
			std::cout << "DoubleRotationRightLeft" << std::endl;
		}

		node->right = SimpleRotationRight(node->right);

		return SimpleRotationLeft(node);
	}

	Node<T>* DoubleRotationLeftRight(Node<T>* node) {

		if (DEBBUGING) {
			std::cout << "DoubleRotationLeftRight" << std::endl;
		}

		node->left = SimpleRotationLeft(node->left);
		return SimpleRotationRight(node);
	}

	public:
	void insert(int value) {
		root = insert(value, root, 0);
	}

	private:
	Node<T>* insert(int value, Node<T>* node, int height) {

		if (node == nullptr) {
			node = new Node<T>(value);
		}

		else if (value < node->value) {
			node->left = insert(value, node->left, height + 1);
		}

		else if (value > node->value) {
			node->right = insert(value, node->right, height + 1);
		}

		else {
			// printf("Erro ao inserir na árvore: '%d' já pertece à árvore.", value);
			// errx(0, "Erro ao inserir na árvore: '%d' já pertece à árvore.", value);
		}

		return Balance(node);
	}

	// int DeleteAVL(int value, AVL* tree) {

	// 	if (tree->root == nullptr) {
	// 		errx(0, "Erro ao remover na árvore: Árvore vazia.");
	// 	}

	// 	return DeleteAVLAux(value, &tree->root);
	// }

	// int DeleteAVLAux(int value, Node<T>** root) {

	// 	int removed;

	// 	if (*root == nullptr) {
	// 		errx(0, "Erro ao remover na árvore: '%d' não pertece à árvore.", value);
	// 	} else if (value < (*root)->value) {
	// 		removed = DeleteAVLAux(value, &(*root)->left);
	// 	} else if (value > (*root)->value) {
	// 		removed = DeleteAVLAux(value, &(*root)->right);
	// 	} else {

	// 		Node<T>* aux = *root;
	// 		removed = (*root)->value;

	// 		if ((*root)->left == nullptr) {
	// 			*root = (*root)->right;
	// 		} else if ((*root)->right == nullptr) {
	// 			*root = (*root)->left;
	// 		} else {
	// 			aux = GreatestToTheLeft(&(*root)->left, *root);
	// 		}

	// 		free(aux);
	// 	}

	// 	return removed;
	// }

	// Node<T>* GreatestToTheLeft(Node<T>** root, Node<T>* parent) {

	// 	Node<T>* removed;

	// 	if ((*root)->right == nullptr) {
	// 		removed = *root;
	// 		parent->value = (*root)->value;
	// 		*root = (*root)->left;
	// 	} else {
	// 		removed = GreatestToTheLeft(&(*root)->right, parent);
	// 	}

	// 	return removed;
	// }

	public:
	Node<T>* search(int value) {
		return search(value, root);
	}

	private:
	Node<T>* search(int value, Node<T>* node) {

		if (node == nullptr) {
			// printf("Erro ao pesquisar na árvore: '%d' não pertece à árvore.", value);
			// errx(0, "Erro ao pesquisar na árvore: '%d' não pertece à árvore.", value);
		}

		else if (value < node->value) {
			node = search(value, node->left);
		}

		else if (value > node->value) {
			node = search(value, node->right);
		}

		return node;
	}

	public:
	void str() {

		if (root == nullptr) {
			throw std::runtime_error("Erro ao printar árvore: Árvore vazia.");
		}

		std::cout << "{ ";
		str(root);
		std::cout << "\b\b }" << std::endl;
	}

	private:
	void str(Node<T>* node) {
		if (node != nullptr) {
			str(node->left);
			std::cout << std::format("{}, ", node->value);
			str(node->right);
		}
	}
};
