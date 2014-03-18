#include "function.h"

/* The function call below is defined in a separate translation unit, so is not usually
 * able to be inlined during compile/link. By compiling both TUs with -flto and linking
 * with the same, gcc's Link-time optimisation is able to perform inter-process optimisation
 * that would otherwise be missed.
 * Note that it doesn't work well with -g.
 */
int main()
{
	return the_function();
}

