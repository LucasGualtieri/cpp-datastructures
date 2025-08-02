#include <iostream>
#include <type_traits>
#include <concepts>

template <typename T>
typename std::enable_if<std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, float>, void>::type
bar() { }

template <typename T>
concept AllowedType = std::same_as<T, int> || std::same_as<T, double>;

template <AllowedType T>
void func() { }

template<typename Type1, typename Type2>
void foo() {

	if constexpr (std::is_same_v<Type1, int>) {
        std::cout << "Tipo: int\n";
    }

	if constexpr (std::is_same_v<Type2, float>) {
        std::cout << "Tipo: float\n";
    }
}

int main() {

	foo<int, float>();
	bar<float>();
	func<float>();

	return 0;
}
