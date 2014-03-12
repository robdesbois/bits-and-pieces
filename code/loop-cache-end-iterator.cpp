/**
 * File provides 2 equivalent function implementations, 1 with a loop that
 * caches the end iterator up-front, the other retrieves it from the container
 * at the end of each iteration.
 * This was clearly represented in the compiled assembly code, however after
 * optimisation (-O2, g++ 4.8.2) the non-caching version had the repeated .end()
 * call optimised out and was equivalent to a simple register compare between
 * iterations.
 */
#include <set>

void cache_it( std::set<int>& s )
{
	int sum = 0;
	for ( auto it = s.begin(), eo = s.end(); it != eo; ++it )
		sum += *it;
}

void no_cache_it( std::set<int>& s )
{
	int sum = 0;
	for ( auto it = s.begin(); it != s.end(); ++it )
		sum += *it;
}

int main()
{
	std::set<int> s{ 1, 5, 4, 8, 2, 3 };
	no_cache_it(s);
	cache_it(s);
}

