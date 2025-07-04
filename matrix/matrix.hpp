#pragma once

#include <initializer_list>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iomanip>

template <typename T>
class Matrix {

	T** matrix;

    public:

	const int height, width;

	Matrix(const std::initializer_list<std::initializer_list<T>>& m) : height(m.size()), width(m.begin()->size()) {

		matrix = (T**)malloc(height * sizeof(T*));

		if (matrix == nullptr) {
			throw std::runtime_error("Memory allocation failed!");
		}

		for (int i = 0; i < height; i++) {

			matrix[i] = (T*)malloc(width * sizeof(T));

			if (matrix[i] == nullptr) {
				throw std::runtime_error("Memory allocation failed!");
			}
		}

		int i = 0;

		for (const std::initializer_list<T>& row : m) {

			int j = 0;

			for (const T& value : row) {
				matrix[i][j] = value;
				j++;
			}

			i++;
		}
	}

	Matrix(int height, int width) : height(height), width(width) {

		matrix = (T**)malloc(height * sizeof(T*));

		if (matrix == nullptr) {
			throw std::runtime_error("Memory allocation failed!");
		}

		for (int i = 0; i < height; i++) {

			matrix[i] = (T*)malloc(width * sizeof(T));

			if (matrix[i] == nullptr) {
				throw std::runtime_error("Memory allocation failed!");
			}
		}
	}

	Matrix(const bool& aux, const int& height, const int& width) : height(height), width(width) {

		matrix = (T**)malloc(height * sizeof(T*));

		if (matrix == nullptr) {
			throw std::runtime_error("Memory allocation failed!");
		}

		if (!aux) {

			for (int i = 0; i < height; i++) {

				matrix[i] = (T*)malloc(width * sizeof(T));

				if (matrix[i] == nullptr) {
					throw std::runtime_error("Memory allocation failed!");
				}
			}
		}

		else {

			for (int i = 0; i < height; i++) {

				matrix[i] = (T*)calloc(width, sizeof(T));

				if (matrix[i] == nullptr) {
					throw std::runtime_error("Memory allocation failed!");
				}
			}
		}
	}

	~Matrix() {

		for (int i = 0; i < height; i++) {
			free(matrix[i]);
		}

		free(matrix);
	}

	bool inBounds(int i, int j) const {
		return 0 <= i && i < height && 0 <= j && j < width;
	}

	bool notInBounds(int i, int j) const {
		return !inBounds(i, j);
	}

    T* operator[](int i) {

        if (i < 0 || i >= height) {
            throw std::out_of_range("Index out of range: " + std::to_string(i));
        }

        return matrix[i];
    }

    // Const version of operator[]
	const T* operator[](int i) const {

		if (i < 0 || i >= height) {
			throw std::out_of_range("Index out of range: " + std::to_string(i));
		}

		return matrix[i];
	}

	std::string str() const {

		std::ostringstream oss;
		int colWidth = 0;

		// First pass: Determine the width needed for each column
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				std::ostringstream temp;
				temp << matrix[i][j];
				colWidth = std::max(colWidth, static_cast<int>(temp.str().length()));
			}
		}

    	oss << "  ";

		for (int i = 0; i < width; i++) {
			oss << std::setw(colWidth) << i << " ";
		}

		oss << "\n";

		// Second pass: Generate the string with alignment
		for (int i = 0; i < height; i++) {
            oss << i << " ";
			for (int j = 0; j < width; j++) {
				oss << std::setw(colWidth) << matrix[i][j];
				if (j < width - 1) oss << " ";
			}
			oss << '\n';
		}

		return oss.str();
	}

	std::string str(bool flag) const {
		std::ostringstream oss;
		int colWidth = 0;

		// First pass: Determine the width needed for each column
		// for (int i = 0; i < height; i++) {
		// 	for (int j = 0; j < width; j++) {
		// 		ostringstream temp;
		// 		if (matrix[i][j] > 10) temp << (matrix[i][j] / 10);
		// 		else if (abs(matrix[i][j]) >= 100) temp << (matrix[i][j] / 100);
		// 		else temp << matrix[i][j];
		// 		colWidth = max(colWidth, static_cast<int>(temp.str().length()));
		// 	}
		// }

		oss << "  ";

		for (int i = 0; i < width; i++) {
			oss << std::setw(colWidth) << i << " ";
		}

		oss << "\n";

		// Second pass: Generate the string with alignment
		for (int i = 0; i < height; i++) {
			oss << i << " ";
			for (int j = 0; j < width; j++) {

				if (10 <= matrix[i][j] && matrix[i][j] < 100 || matrix[i][j] == -2) {
					// oss << std::setw(colWidth) << matrix[i][j];
					oss << std::setw(colWidth) << (matrix[i][j] / 10);
					if (j < width - 1) oss << " ";
				}

                else if (abs(matrix[i][j]) >= 100) {
					oss << std::setw(colWidth) << "*";
					if (j < width - 1) oss << " ";
				}

				else {
					// oss << std::setw(colWidth) << " ";
					oss << std::setw(colWidth) << ".";
					if (j < width - 1) oss << " ";
				}
			}
			oss << '\n';
		}

		return oss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
		os << m.str();
		return os;
	}

    // Iterator for Matrix
    class Iterator {
        Matrix& matrix;
        int row, col;

        public:
        Iterator(Matrix& mat, int r = 0, int c = 0) : matrix(mat), row(r), col(c) {}

        bool operator!=(Iterator& other) {
            return row != other.row || col != other.col;
        }

        Iterator& operator++() {
            col++;
            if (col >= matrix.width) {
                col = 0;
                row++;
            }
            return *this;
        }

        T& operator*() {
            return matrix[row][col];
        }
    };

    Iterator begin() {
        return Iterator(*this, 0, 0);
    }

    Iterator end() {
        return Iterator(*this, height, 0);
    }

};
