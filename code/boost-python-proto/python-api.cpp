#include <boost/python.hpp>

/**

Build & test steps
------------------

compile all object files with
    -fPIC
compile the module interface file (this one) with (e.g.)
    -fPIC -I/usr/include/python2.7/
build the shared library with (e.g.)
    -shared -Wl,-soname,"my_module.so" -lpython2.7 -lboost_python

now run python in the same directory as the .so and enter
    import my_module

done!

*/


int get_42()
{
    return 42;
}

// Library name MUST match module name, e.g. my_module must be in my_module.so
BOOST_PYTHON_MODULE(my_module)
{
    def("get_42", get_42);
}
