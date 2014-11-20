#include <functional>

template<class T, class Cmp = std::less<T> >
bool is_less(T a, T b, Cmp cmp = Cmp())
{
	return cmp(a, b);
}

int main()
{
	if (is_less(1, 2))
		return 0;
	else return 1;
}

