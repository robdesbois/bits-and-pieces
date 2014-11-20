#include <boost/signals2.hpp>
#include <atomic>
#include <iostream>
#include <thread>


namespace dd_process {
	namespace state {
		std::atomic<bool> keep_running(true);
	}

	namespace signal {
		boost::signals2::signal<void()> shutdown;
		boost::signals2::signal<void()> print_state;
	}


	void stop_running() {
		dd_process::state::keep_running = false;
	}

	bool keep_running() {
		return state::keep_running;
	}


	void setup_signals_and_slots() {
		signal::shutdown.connect( stop_running );
		signal::print_state.connect( [] { std::cout << "Everything is A-OK" << std::endl; } );
	}
}



int main()
{
	namespace signal = dd_process::signal;

	dd_process::setup_signals_and_slots();

	while ( dd_process::keep_running() ) {
		int c = std::cin.get();

		if (!std::cin) {
			std::cout << "error" << std::endl;
			signal::shutdown();
		}

		switch (c) {
			case 'q':
				signal::shutdown();
				break;

			case 'p':
				signal::print_state();
				break;

			default:
				std::cout << "unknown char" << std::endl;
		}
	}
}
