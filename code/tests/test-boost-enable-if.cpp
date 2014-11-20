#include <boost/utility/enable_if.hpp>
#include <iostream>
#include <boost/type_traits.hpp>

// Primary template provides a defaulted 'Enable' parameter for use by explicit specializations
// Defaulted to avoid poluting the client-facing interface
template<class T, class Enable = void>
struct printer
{
	void operator()(const T& v)
	{
		std::cout << "normal: " << v << std::endl;
	}
};

// Explicit specialization.
// The compiler will attempt to instantiate this for any use of the template:
// * For an integral class enable_if<...>::type is defined and so the specialization is instantiated.
// * For a non-integral class enable_if<...>::type is undefined, and so this specialization is
// rejected *without causing a compiler error*, and the primary template is used instead.
//
// This is SFINAE: Substitution Failure Is Not An Error, i.e. the failure to substitute T with a
// non-integral type in itself does not cause a compiler error.
template<class T>
struct printer<T, typename boost::enable_if< typename boost::is_integral<T>::type >::type>
{
	void operator()(const T& v)
   {
      std::cout << "integral: " << v << std::endl;
	}
};


int main()
{
	printer<std::string> p;
	p("hello string!");
	printer<long> pl;
	pl(42247);
}

