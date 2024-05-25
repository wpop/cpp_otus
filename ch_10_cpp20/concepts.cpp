#include <concepts>
#include <set>




namespace solution
{
	template <typename T>
	concept LessComparable = requires(T a, T b) {
		{ a < b } -> std::convertible_to<bool>;
	};
}


int main()
{
	return 0;
}