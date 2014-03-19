#pragma once

#include <chrono>
#include <iostream>


template<class> constexpr const char* chrono_units();
template<> constexpr const char* chrono_units<std::nano>()     { return "ns"; }
template<> constexpr const char* chrono_units<std::micro>()    { return "us"; }
template<> constexpr const char* chrono_units<std::milli>()    { return "ms"; }
template<> constexpr const char* chrono_units<std::ratio<1>>() { return "s"; }

template<class Rep, class Period>
std::ostream& operator<<(std::ostream& os, const std::chrono::duration<Rep,Period>& d) {
	return os << d.count() ;//<< chrono_units<Period>();
}

namespace rob { namespace perf {
	using clock_t    = std::chrono::high_resolution_clock;
	using mark_t     = clock_t::time_point;
	using duration_t = clock_t::duration;
	
	mark_t mark() {
		return clock_t::now();
	}
}}