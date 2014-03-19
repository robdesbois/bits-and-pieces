#pragma once
#include <cxxabi.h>
#include <iostream>
#include <string>
#include <typeinfo>


namespace rob {
	/** Obtain demangled typename. */
	template< class T >
	std::string demangle(const T&) {
		size_t size(256);
		char*  buf  = (char*)malloc(size);
		int  result = 0;
		abi::__cxa_demangle( typeid(T).name(), buf, &size, &result );
		
		std::string demangled;
		if ( result != 0 ) {
			demangled = "unable to demangle";
			std::cout << "no demangle: " << result << std::endl;
		}
		else
			demangled = buf;
		
		free(buf);
		return demangled;
	}
}